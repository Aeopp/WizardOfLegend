#include "pch.h"
#include "FireDragon.h"
#include "sound_mgr.h"
#include "collision_mgr.h"
#include "Color.h"
#include "Bmp_mgr.h"
#include "helper.h"
#include "game.h"
#include "Bmp.h"

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
	sp_collision->PushForce = 10.f;
	sp_collision->bCollision = true;
	sp_collision->bHitEffect = true;
	sp_collision->bTileHitEffect = true;
	sp_collision->bCollision = true;


	Duration = 10.f;

	PaintSizeX = 180;
	PaintSizeY = 180;
	 Scale = 0.93;
	 ColIdx = 0;
	 RowIdx = 0;
	 P_Scale = 0.93;
	 ParticleMaxNum = 20;

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
	amplitude = 100.f;
};

Event FireDragon::update(float dt)
{
	Duration -= dt;
	ParticleDelta -= dt;

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
		RAND_SOUNDPLAY("FIRE_DRAGON_DIE", { 0,3 }, 1.f, false);
	}
	if (ParticleLocationDQ.empty() && Duration<0)
	{
		bDie = true;
		return Event::Die;
	}

	Event _event = object::update(dt);

	if (!_transform)return Event::Die;

	// 현재 각도의 회전 스피드 에 델타타임을 곱해서 각도를 점점 증가시키거나
	// 감소시킨다.
	CurrentAngle += (RotationSpeed * MyFactor) * dt;
	// 각도가 Max 나 Min 에 도달 할 시 각도의 증가나 감소의 여부를 결정한다.
	if (CurrentAngle > AngleMax )
	{
		 CurrentAngle = AngleMax; 
		MyFactor *= -1.f;
	}
	else if (CurrentAngle < AngleMin)
	{
		CurrentAngle = AngleMin;
		MyFactor *= -1.f;
	}
	 // 현재 각도로부터 벡터를 만듬.
	vec NewDir = math::dir_from_angle(CurrentAngle);
	// 데카르트 좌표계가 아닌 화면 좌표계 이기 때문에 y 를 뒤집는다.
	NewDir.y *= -1;

	_transform->_dir = NewDir;
	// 계산한 방향에 스피드를 곱해서 위치에 더한다.
	_transform->_location += (_transform->_dir * Speed * dt);

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

	if (ColIdx == 0)
	{

		int i = 0;
	}
	GdiTransparentBlt(hdc, DestLoc.x, DestLoc.y,
		PaintSizeX *Scale, PaintSizeY*Scale, sp_DragonImg->Get_MemDC(),
		ColIdx * PaintSizeX, RowIdx * PaintSizeY,
		PaintSizeX, PaintSizeY, COLOR::MRGENTA());
}

void FireDragon::HitTile(RECT TileRt)
{
	object::HitTile(TileRt);

	collision_mgr::instance().HitEffectPush(_transform->_location,
		0.5f);

	RAND_SOUNDPLAY("WALL_HITTED_FIREDRAGON", { 0,2 }, 1.f, false);

	auto sp_collision = _collision_component.lock();
	if (!sp_collision)return;
	bAttacking = false;
	sp_collision->bSlide = false;
	sp_collision->bCollision = false;

	bWallHited = true;
	Duration = ParticleLocationUpdateDelta* ParticleMaxNum + 0.05f;
}

void FireDragon::SetUp(vec Location, vec Dir)
{
	if (!_transform)return;

	_transform->_location = Location;
	_transform->_dir = Dir;
	CurrentAngle = math::AngleFromVec(Dir);
	AngleMin = CurrentAngle - 45;
	AngleMax = CurrentAngle + 45;
	
	CalcSpriteFromAngle();
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



