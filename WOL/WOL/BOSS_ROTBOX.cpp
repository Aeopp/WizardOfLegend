#include "pch.h"
#include "BOSS_ROTBOX.h"
#include "object_mgr.h"
#include "BOSS_BOX.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "collision_component.h"
#include "collision_mgr.h"
#include "sound_mgr.h"
#include "BOTTOM_HOLE.h"


void BOSS_ROTBOX::initialize()
{
	object::initialize();

	_collision_component =
		collision_mgr::instance().insert(_ptr, collision_tag::EMonsterAttack,
			ERect);

	auto sp_collision = _collision_component.lock();
	if (!sp_collision)return;

	sp_collision->bCollisionTargetPushFromForce = true;
	sp_collision->PushForce = 40.f;

	sp_collision->_size = { 50,50 };
	sp_collision->bRender = true;
	sp_collision->bSlide = false;
	sp_collision->bSlide = true;

	bAttacking = true;

	PaintSizeX = 110;
	PaintSizeY = 155;
	ScaleX = 1.f;
	ScaleY = 1.f;

	SOIL_PaintSizeX = 60;
	SOIL_PaintSizeY = 60;
	SOIL_ScaleX = 1;
	SOIL_ScaleY = 1;

	sp_Bmp = Bmp_mgr::instance().Find_Image_SP(L"BOSS_BOXATTACK");
	sp_SOILBmp = Bmp_mgr::instance().Find_Image_SP(L"SOIL_EFFECT");

	if (!sp_Bmp)return;

	ObjectTag = object::Tag::monster_attack;

	Attack = { 50,60 };

	UniqueID = EObjUniqueID::BOSS_BOXATTACK;

	Speed = 2000.f;

	SOIL_Cols.reserve(SOIL_NUM);
	static auto RAND_SOIL_COL_GENERATE = [] {
		return  std::pair<int32_t, vec>{ math::Rand<int>({ 0,3 }), math::RandVec() * 120 }; };

	std::generate_n(std::back_inserter(SOIL_Cols), SOIL_NUM, RAND_SOIL_COL_GENERATE);
}

Event BOSS_ROTBOX::update(float dt)
{
	Duration -= dt;
	SOIL_Duration -= dt;
	InitDuration -= dt;
	RotationDuration -= dt;

	if (Duration < 0)
	{
		return Event::Die;
	}

	if (InitDuration > 0)
	{
		_transform->_location.y -= (FlyTimeInAddY * dt);
		InitLocation.y -= (FlyTimeInAddY * dt);
		RotationCenter.y-= (FlyTimeInAddY * dt);
	}

	if (RotationDuration > 0.35 && SOIL_Duration<0)
	{
		float RotationSpeedIncrement = 50.f;
		RotationDir = math::rotation_dir_to_add_angle(RotationDir, RotationSpeed * dt);
		_transform->_location = RotationCenter + RotationDir * DistanceOfCenter;
		RotationSpeed += dt * RotationSpeedIncrement;
	}



	if (InitDuration < 0 && !bLaunch && RotationDuration < 0 )
	{
		auto sp_Target = _owner.lock();
		if (!sp_Target) return Event::None;
		if (!sp_Target->_transform)return Event::None;

		vec Dis = sp_Target->_transform->_location - _transform->_location;
		vec Dir = Dis.get_normalize();
		_transform->_dir = Dir;
		bLaunch = true;
		auto sp_collision = _collision_component.lock();
		if (!sp_collision)return Event::None;
		sp_collision->bCollision = true;

	}

	if (bLaunch) {
		
		_transform->_location += _transform->_dir * dt * Speed;
	}

	return Event::None;
}

void BOSS_ROTBOX::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!_transform)return;
	if (!sp_Bmp)return;


	vec DestLoc = _transform->_location - camera_pos;
	int WorldSizeX = (PaintSizeX * ScaleX);
	int WorldSizeY = (PaintSizeY * ScaleY);

	GdiTransparentBlt(hdc
		, DestLoc.x - WorldSizeX / 2, DestLoc.y - WorldSizeY / 2
		, WorldSizeX, WorldSizeY
		, sp_Bmp->Get_MemDC()
		,
		0, 0
		, PaintSizeX, PaintSizeY
		, COLOR::MRGENTA());


	if (SOIL_Duration < 0)return;


	SOILAnimTick += DeltaTime;
	if (SOILAnimTick > 0.7f)
	{
		SOILAnimRowDecrement--;
		SOILAnimTick = 0;
	}
		

	for (int RowIdx = 0; RowIdx < SOIL_NUM; ++RowIdx)
	{
		int ColIdx = SOIL_Cols[RowIdx].first;

		vec SOIL_Rand_Loc = (InitLocation- camera_pos) + SOIL_Cols[RowIdx].second;

		int SOIL_WorldSizeX = (SOIL_PaintSizeX * SOIL_ScaleX);
		int SOIL_WorldSizeY = (SOIL_PaintSizeY * SOIL_ScaleY);

		int SoilRowIdx = max(RowIdx + SOILAnimRowDecrement,0);

		GdiTransparentBlt(hdc
			, SOIL_Rand_Loc.x - SOIL_WorldSizeX / 2, SOIL_Rand_Loc.y - SOIL_WorldSizeY / 2
			, SOIL_WorldSizeX, SOIL_WorldSizeY
			, sp_SOILBmp->Get_MemDC()
			,
			ColIdx * SOIL_PaintSizeX, SoilRowIdx * SOIL_PaintSizeY
			, SOIL_PaintSizeX, SOIL_PaintSizeY
			, COLOR::MRGENTA());
	}
}

void BOSS_ROTBOX::late_initialize(vec InitLocation, float InitDuration,
	std::weak_ptr<class object> wp_AttackTarget,vec RotationCenter,float RotationDuration)
{
	if (!_transform)return;
	this->InitLocation = _transform->_location = InitLocation;
	this->InitDuration = InitDuration;
	_owner = wp_AttackTarget;
	RotationDir = (InitLocation - RotationCenter).get_normalize(); 
	this->RotationDuration = RotationDuration;
	this->RotationCenter = RotationCenter;
	auto sp_collision = _collision_component.lock();
	if (!sp_collision)return;
	sp_collision->bCollision = false ;
	auto sp_Hole = object_mgr::instance().insert_object<BOTTOM_HOLE>(InitLocation, Duration);
	if (!sp_Hole)return;
}

void BOSS_ROTBOX::HitTile(RECT rt)
{
collision_mgr::instance().HitEffectPush(_transform->_location, 0.5f);
bDie = true;
Duration = -1;

}

uint32_t BOSS_ROTBOX::get_layer_id() const&
{
	return layer_type::EEffect;
}





