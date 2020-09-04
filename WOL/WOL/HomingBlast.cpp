#include "pch.h"
#include "HomingBlast.h"
#include "Bmp.h"
#include "Bmp_mgr.h"
#include "collision_component.h"
#include "collision_mgr.h"
#include "sound_mgr.h"


void HomingBlast::initialize()
{
	object::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EMonsterAttack,
		ECircle);
	auto sp_collision = _collision_component.lock();
	if (!sp_collision)return;

	sp_collision->bCollisionTargetPushFromForce = true;
	sp_collision->PushForce = 50.f;

	sp_collision->_size = { 60.f ,60.f};
	sp_collision->bRender = true;
	sp_collision->bSlide = false;
	sp_collision->bSlide = true;

	bAttacking = true;

	PaintSizeX = 200;
	PaintSizeY = 250;
	ScaleX = 0.45f;
	ScaleY = 0.45f;

	sp_Bmp = Bmp_mgr::instance().Find_Image_SP(L"ICE_BLAST");
	if (!sp_Bmp)return;

	ObjectTag = object::Tag::monster_attack;
	CurrentColIdx = 0;

	Attack = { 50,60 };

	sound_mgr::instance().RandSoundKeyPlay("ICE_BLAST", { 1,4 }, 1.f);

	UniqueID = EObjUniqueID::ICEBLAST;

	HomingRotationAddSpeed = 0.3f;
	HomingRotationCurrentSpeed = 1.f;
	Speed = 650;
	
	std::fill_n(std::back_inserter(ParticlesPositions), 6, _transform->_location);
	

	ICE_EffectPlay();

}

Event HomingBlast::update(float dt)
{
	CurrentParticlePosUpdateDelta -= dt;
	Duration -= dt;
	CurrentAnimRemainTime -= dt;
	AnimInitTime -= dt;

	if (Duration < 0)
	{
		ICE_EffectPlay;
		return Event::Die;
	}

	auto sp_Target = _owner.lock();
	if (!sp_Target)return Event::Die;
	if (!sp_Target->_transform)return Event::Die;

	vec CurrentDir = _transform->_dir;
	vec TargetDir = (sp_Target->_transform->_location - _transform->_location).get_normalize();

	float Z = CurrentDir.CrossZ(TargetDir);
	float Dot = CurrentDir.dot(TargetDir);

	float AngleDegree = math::radian_to_degree(std::acosf(Dot));

	// 시계 방향 회전
	if (Z >0)
		AngleDegree *= -1;

	_transform->_dir = 
	math::rotation_dir_to_add_angle(CurrentDir, AngleDegree * dt * 
		HomingRotationCurrentSpeed);

	HomingRotationCurrentSpeed += HomingRotationAddSpeed * dt;

	_transform->_location += _transform->_dir * dt * Speed;
	


	if (CurrentParticlePosUpdateDelta < 0)
	{
		CurrentParticlePosUpdateDelta = ParticlePosUpdateCycle;
		ParticlesPositions.push_back(_transform->_location);
		ParticlesPositions.pop_front();
	}

	if (Duration < 1.6)
	{
		ParticlesIdxPopTick -= dt;
		if (ParticlesIdxPopTick < 0)
		{
			ParticlesIdxPopTick = 0.2f;
			if(!ParticlesColIdxs.empty())
				ParticlesColIdxs.pop_back();
		}
	}

	return Event::None;
}

void HomingBlast::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!_transform)return;
	if (!sp_Bmp)return;

	AnimUpdate();

	ICE_EffectPlay();

	
	for (auto& PastPos : ParticlesPositions)
	{
		vec DestLoc = PastPos - camera_pos;
		int WorldSizeX = (PaintSizeX * ScaleX);
		int WorldSizeY = (PaintSizeY * ScaleY);
		DestLoc.x -= WorldSizeX;
		DestLoc.y -= WorldSizeY;

		GdiTransparentBlt(hdc
			, DestLoc.x, DestLoc.y
			, WorldSizeX, WorldSizeY
			, sp_Bmp->Get_MemDC()
			, //CurrentColIdx * PaintSizeX, 0
			//CurrentColIdx* PaintSizeX, 0
			CurrentColIdx * PaintSizeX, 0
			, PaintSizeX, PaintSizeY
			, COLOR::MEGENTA());
		
	}
}

void HomingBlast::late_initialize(std::weak_ptr<object>Target, vec InitLocation, vec InitDir)
{
	if (!_transform)return;

	_transform->_location = InitLocation;
	_transform->_dir = InitDir;
	_owner = Target;
}

uint32_t HomingBlast::get_layer_id() const&
{
	return layer_type::EObject;
}

void HomingBlast::AnimUpdate()
{
	if (CurrentAnimRemainTime < 0)
	{
		CurrentAnimRemainTime= AnimDelta;
		CurrentColIdx++;
	}

	if (AnimInitTime < 0 && Duration > 0.3)
		CurrentColIdx = 2;
}

void HomingBlast::ICE_EffectPlay()
{


	if (!_transform)return;

	vec MyLocation = _transform->_location;

	collision_mgr::instance().HitEffectPush(MyLocation + math::RandVec() * math::Rand<int>({ -20,20 }), 0.2f);

}
