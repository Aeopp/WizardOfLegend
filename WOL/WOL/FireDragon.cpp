#include "pch.h"
#include "FireDragon.h"
#include "sound_mgr.h"
#include "collision_mgr.h"
#include "Color.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "Burning_Interface.h"
#include "Effect.h"
#include "object_mgr.h"


void FireDragon::initialize()
{
	object::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EPlayerAttack, ECircle);

	auto sp_collision = _collision_component.lock();

	if (!sp_collision)return;

	sp_collision->_size = { 40,40};
	sp_collision->bSlide = true;
	sp_collision->bCollisionSlideAnObject = false;
	ObjectTag = object::Tag::player_attack;
	sp_collision->bCollisionTargetPushFromForce = true;
	sp_collision->PushForce = 3.f;
	sp_collision->bCollision = true;
	sp_collision->bHitEffect = true;
	sp_collision->bTileHitEffect = true;
	sp_collision->bCollision = true;


	Duration = 3.f;

	PaintSizeX = 180;
	PaintSizeY = 180;
	 Scale = 0.93;
	 ColIdx = 0;
	 RowIdx = 0;
	 P_Scale = 0.93;
	 ParticleMaxNum = 8;

	// 자기자신의 회전속도임

	 Speed = 1666 * 0.8f;
	RotationSpeed = 600 * 0.8f;
	Attack = { 20,40 };
	bAttacking = true;
	MyFactor = FireDragon::factor *= -1.f;
	UniqueID = EObjUniqueID::FIREDRAGON;

	RAND_SOUNDPLAY("FIRE_DRAGON", { 1,3 }, 1.f, false);

	sp_DragonImg = Bmp_mgr::instance().Find_Image_SP(L"SKILL_FIREDRAGON_COM");
	sp_ParticleImg= Bmp_mgr::instance().Find_Image_SP(L"FIRE_PARTICLE");

	//////////////////
	amplitude = 70;
};

Event FireDragon::update(float dt)
{
	Duration -= dt;
	ParticleDelta -= dt;
	T += dt * 12;

	if (ParticleDelta < 0)
	{
		ParticleDelta = ParticleLocationUpdateDelta;
		if (ParticleLocationDQ.size() > ParticleMaxNum ||
			Duration<0 || bWallHited)
		{
			if(!ParticleLocationDQ.empty())
			ParticleLocationDQ.pop_front();
		}
		if (Duration > 0 && !bWallHited)
		{
			ParticleLocationDQ.push_back({ _transform->_location,
			math::Rand<int>({0,3}),0 ,ParticelAnimDelta });
		}
	}
	if (Duration<0)
	{
		if(!bDieSoundPlayed)
		{
			RAND_SOUNDPLAY("FIRE_DRAGON_DIE", { 0,3 }, 1.f, false);
			bDieSoundPlayed = true;
		}
	}
	if (ParticleLocationDQ.empty() && Duration<0)
	{
		bDie = true;
		return Event::Die;
	}

	Event _event = object::update(dt);

	if (!_transform)return Event::Die;

	// 커런트 앵글과 트랜스폼 방향 반드시 갱신
	// 커런트 앵글 360 으로 트랜스폼 방향 용의 머리를 결정 하도록

	LinePos += LineDir * Speed * dt;
	float y = std::sinf(T);
	;
	vec Pos = LinePos + (PerpendicularDir * (y * amplitude * MyFactor ) );
	_transform->_location = Pos;


	CurrentAngle = InitAngle + (45 * y);
	_transform->_dir = math::dir_from_angle(CurrentAngle);
	return _event;
}

uint32_t FireDragon::get_layer_id() const&
{
	return layer_type::EEffect;
}

void FireDragon::render(HDC hdc, vec camera_pos, vec size_factor)
{
	object::render(hdc, camera_pos, size_factor);

	if (!_transform)return; 
	if (!sp_DragonImg)return;
	if (!sp_ParticleImg)return;
	for (auto& [Particle_Loc,Row,Col,AnimDT] : ParticleLocationDQ)
	{
		int ParticlePaintSize = 70;
		vec PLoc = Particle_Loc;
		PLoc -= camera_pos;
		PLoc -= (ParticlePaintSize * P_Scale) / 2;

		AnimDT -= DeltaTime;
		if (AnimDT < 0)
		{
			AnimDT = ParticelAnimDelta; 
			Col++;
			if (Col > 4)
				Col = 0;
		}
		GdiTransparentBlt(hdc, PLoc.x, PLoc.y,
			ParticlePaintSize* P_Scale, ParticlePaintSize* P_Scale, sp_ParticleImg->Get_MemDC(),
			Col * ParticlePaintSize, Row* ParticlePaintSize,
		ParticlePaintSize, ParticlePaintSize, COLOR::MRGENTA());
	}

	if (bWallHited)return;
	vec DestLoc = _transform->_location;
	DestLoc -= camera_pos;
	DestLoc.x -= (PaintSizeX * Scale)/2;
	DestLoc.y -= (PaintSizeY * Scale)/2;

	CalcSpriteFromAngle();

	GdiTransparentBlt(hdc, DestLoc.x, DestLoc.y,
		PaintSizeX *Scale, PaintSizeY*Scale, sp_DragonImg->Get_MemDC(),
		ColIdx * PaintSizeX, RowIdx * PaintSizeY,
		PaintSizeX, PaintSizeY, COLOR::MRGENTA());
}

void FireDragon::HitTile(RECT TileRt)
{
	object::HitTile(TileRt);

	if(!bWallHited)
	{
		collision_mgr::instance().HitEffectPush(_transform->_location,
			0.5f);

		RAND_SOUNDPLAY("WALL_HITTED_FIREDRAGON", { 0,2 }, 1.f, false);

		auto sp_collision = _collision_component.lock();
		if (!sp_collision)return;
		bAttacking = false;
		sp_collision->bSlide = false;
		sp_collision->bCollision = false;

		bWallHited = true;
		Duration = ParticleLocationUpdateDelta * ParticleMaxNum + 0.05f;
	}
}

void FireDragon::Hit(std::weak_ptr<object> _target)
{
	auto sp_Target = _target.lock();
	if (!sp_Target)return;

	if (Burning_Interface::probability_calculation()
		&& sp_Target->ObjectTag == object::Tag::monster)
	{
		auto sp_EnableObject = std::dynamic_pointer_cast<Burning_Interface>(sp_Target);
		if (!sp_EnableObject)return;
		if (!_transform)return;

		vec MsgLocation = _transform->_location;

		sp_EnableObject->_Burning_Info.burn(MsgLocation);
	}

}

void FireDragon::SetUp(vec Location, vec Dir)
{
	if (!_transform)return;

	_transform->_location = Location;
	_transform->_dir = Dir;
	InitAngle=CurrentAngle = math::AngleFromVec(Dir);
	
	CalcSpriteFromAngle();

	PerpendicularDir = math::rotation_dir_to_add_angle(Dir, 90);
	LineDir = Dir;
	LinePos = Location;
	T = 0;
	
	static const size_t ParticleNum = 4;
	static const std::wstring ImgKey = L"FIRE_PARTICLE";
	static const float ParticleDuration = 0.6f;
	// launch 시 파티클 위치 , 파티클 흩날릴 방향 , 파티클 스프라이트 인덱스
	std::vector<std::tuple<vec,vec,int32_t>> ParticleInfos;
	ParticleInfos.reserve(ParticleNum);
	float MyFactor = this->MyFactor;
	auto ParticleInfoGenerate = [MyFactor,Location, Dir]()
		->typename decltype(ParticleInfos)::value_type
	{
		const float DistanceMin = 100;
		const std::pair<int, int> RandRange = { -100,100};
		
		int32_t Row = math::Rand<int32_t>({0,3});
		vec InitDir = math::rotation_dir_to_add_angle((Dir * -1), 45 * MyFactor);
		vec InitLocation = Location + (InitDir * DistanceMin);
		vec RandDir = math::rotation_dir_to_add_angle(InitDir, math::Rand<int>({0,45}));
		InitLocation +=  (RandDir * math::Rand<int32_t>(RandRange));
		return { InitLocation , RandDir   ,Row  };
	};

	std::generate_n(std::back_inserter(ParticleInfos),
		ParticleNum, std::move(ParticleInfoGenerate));

	for(const auto& [Location,Dir,IDX] : ParticleInfos)
	{
		int ImgLocationX = Location.x;
		int ImgLocationY =  Location.y;
		int AnimRowIndex = IDX;
		
		auto sp_Effect = 	object_mgr::instance().insert_object<Effect>(
			ImgLocationX, ImgLocationY, ImgKey, layer_type::EEffect,
			5, AnimRowIndex, ParticleDuration, ParticleDuration,
			70, 70, 0.8f, 0.8f , Dir);
	}
	//void late_initialize(int ImgLocationX, int ImgLocationY,
	//	std::wstring ImgKey, layer_type layer_ID, int AnimColNum,
	//	int AnimRowIndex, float Duration, float AnimDuration,
	//	int PaintSizeX, int PaintSizeY, float ScaleX, float ScaleY);
}

void FireDragon::CalcSpriteFromAngle()
{
	int IDX = abs(CurrentAngle / 19);

	if (_transform->_dir.x > 0)
	{
		RowIdx = 0; 
		ColIdx = SpriteAngleColMap.second[IDX];
	}
	else
	{
		RowIdx = 1;
		ColIdx = SpriteAngleColMap.first[IDX]; 
	}
}



