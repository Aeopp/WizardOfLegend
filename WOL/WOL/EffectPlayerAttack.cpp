#include "pch.h"
#include "sound_mgr.h"

#include "EffectPlayerAttack.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "collision_mgr.h"



void EffectPlayerAttack::AttackReady(float Duration, float AnimDuration
,float PushForce, vec InitLocation, AttackNumber Num, float AnimAngle)
{
	if (!_transform) return;

	Angle = AnimAngle;
	AnimRowIndex = (int)Angle / 45;
	this->Duration = Duration;
	this->AnimDuration = AnimDuration;
	this->_transform->_location = InitLocation;
	this->ImgLocationX = InitLocation.x;
	this->ImgLocationY = InitLocation.y;
	this->CurrentAttackNum = Num;

	UniqueID = EObjUniqueID::NormalAttack;
	ObjectTag = object::Tag::player_attack;
	bAttacking = true;
	
	auto sp_comp = _collision_component.lock();
	if (!sp_comp)return;
	sp_comp->bCollisionTargetPushFromForce = true;
	sp_comp->PushForce = PushForce;
	sp_comp->bCollision = true;
	sp_comp->bSlide = false;
	bRender = true;
	AnimT = AnimDuration;

}

void EffectPlayerAttack::AttackEnd()
{
	bAttacking = false;
	auto sp_comp = _collision_component.lock();
	if (!sp_comp)return;
	sp_comp->bSlide = false;
	sp_comp->bCollision = false;
	sp_comp->bCollisionTargetPushFromForce = false;
	sp_comp->PushForce = 0.f;

}

Event EffectPlayerAttack::update(float dt)
{
	if (!bRender)return Event::None;
	Event _event = object::update(dt);
	SoundTick -= dt;
	Duration -= dt;
	AnimDelta -= dt;
	AnimT -= dt;

	if (AnimT < 0)
	{
		bRender = false;
	}
	if (Duration < 0) {
		AttackEnd();
	}

	auto sp_owner = _owner.lock();
	if (!sp_owner)return _event;

	int AnimStart{}, AnimEnd{};
	switch (CurrentAttackNum)
	{
	case _1:
		AnimStart = 0;
		AnimEnd = 3;
		break;
	case _2:
		AnimStart = 4;
		AnimEnd = 7;
		break;
	default:
		break;
	}

	if (AnimDelta < 0)
	{
		AnimDelta = (float)AnimDuration / 7.f;
		++CurrentCol;
		if (CurrentCol > AnimEnd)
		{
			CurrentCol = AnimStart;
		}
	}

	return _event;
}

void EffectPlayerAttack::render(HDC hdc, vec camera_pos, vec size_factor)
{
	object::render(hdc, camera_pos, size_factor);

	if (!EffectImg)return;
	if (!bRender)return;

	//월드 사이즈
	int wsx = (PaintSizeX * ScaleX);
	int wsy = (PaintSizeY * ScaleY);

	vec  v = { ImgLocationX,ImgLocationY };
	v -= camera_pos;
	
	AnimRowIndex = (int)Angle / 45;

	GdiTransparentBlt(hdc, v.x - wsx / 2, v.y - wsy / 2,
		wsx, wsy, EffectImg->Get_MemDC(), PaintSizeX * CurrentCol, PaintSizeY * AnimRowIndex
		, PaintSizeX, PaintSizeY, ColorKey);
}

void EffectPlayerAttack::initialize()
{
	Effect::initialize();

	_collision_component = 
	collision_mgr::instance().insert(_ptr, collision_tag::EPlayerAttack, ECircle);

	Attack = { 30,40 };
	UniqueID = EObjUniqueID::NormalAttack;

		// 콜리전 셋업
	{
		auto sp_collision = _collision_component.lock();

		if (!sp_collision)return;
		sp_collision->bCollisionTargetPushFromForce = true;
		sp_collision->PushForce = 20.f;

		sp_collision->_size = { 72 ,72};
		sp_collision->bRender = true;
		sp_collision->bSlide = false;
		sp_collision->bCollision = true;
		bRender = false; 
	};
}

void EffectPlayerAttack::Hit(std::weak_ptr<class object> wp_Target)
{
	Effect::Hit(wp_Target);

	if (SoundTick < 0)
	{
		//SOUNDPLAY("HIT_SOUND_NORMAL", 1.f, false);
		SoundTick = 0.05f;
	}
	
}
