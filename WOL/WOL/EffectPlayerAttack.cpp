#include "pch.h"
#include "EffectPlayerAttack.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "collision_mgr.h"

void EffectPlayerAttack::AttackStart(float Duration, float AnimDuration
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

	id = object::ID::player_attack;
	bAttacking = true;
	
	auto sp_comp = _collision_component.lock();
	if (!sp_comp)return;
	sp_comp->bCollisionTargetPushFromForce = true;
	sp_comp->PushForce = PushForce;
	sp_comp->bCollision = true;
	sp_comp->bSlide = false;
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
	if (!bAttacking)return Event::None;

	Event _event = object::update(dt);

	Duration -= dt;
	AnimDelta -= dt;
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
		AnimDelta = (float)AnimDuration / 4;
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
	if (!bAttacking)return;

	object::render(hdc, camera_pos, size_factor);

	if (!EffectImg)return;

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

	Attack = { 8,13 };

		// 콜리전 셋업
	{
		auto sp_collision = _collision_component.lock();

		if (!sp_collision)return;
		sp_collision->bCollisionTargetPushFromForce = true;
		sp_collision->PushForce = 30.f;

		sp_collision->_size = { 45.f,45.0f };
		sp_collision->bRender = true;
		sp_collision->bSlide = false;
		sp_collision->bCollision = true;
	};
}
