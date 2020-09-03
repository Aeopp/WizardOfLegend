#include "pch.h"
#include "SwordManAttack.h"
#include "shield.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "collision_component.h"
#include "collision_mgr.h"


void SwordManAttack::EffectStart(vec dir, vec location)
{
	_Dir = math::checkDir(dir);
	CurrentAttackDuration = DefaultAttackDuration = 0.1f;
	auto sp_comp = _collision_component.lock();
	if (!sp_comp)return;
	sp_comp->bCollision = true;
	if (!_transform)return;
	_transform->_location = location;

	switch (_Dir)
	{
	case math::EDir::left:
		AnimRowIndex = 0;
		break;
	case math::EDir::right:
		AnimRowIndex = 1;
		break;
	case math::EDir::up:
		AnimRowIndex = 4;
		break;
	case math::EDir::down:
		AnimRowIndex = 2;
		break;
	default:
		break;
	}
	ImgLocationX = location.x;
	ImgLocationY = location.y;
	bRender = true; 
	Attack = { 40,50 };
	bAttacking = true;

	AnimDuration = 0.1f;

	ScaleX = 0.9f;
	ScaleY = 0.9f;
	AnimColNum = 4;
};

Event SwordManAttack::update(float dt)
{
	Event _event = object::update(dt);
	if (!_owner.lock()) { return Event::Die; }

	CurrentAttackDuration -= dt;
	if (CurrentAttackDuration < 0)
	{
		auto sp_comp = _collision_component.lock();
		if (!sp_comp)return _event;
		sp_comp->bCollision = false;
		bAttacking = false;
	}

	AnimDuration -= dt;
	if (AnimDuration < 0)
	{
		bRender = false;
		_transform->_location = vec{ 0,0 };
	}

	AnimDelta -= dt;
	if (AnimDelta < 0)
	{
		AnimDelta = (float)AnimDuration / ((float)AnimColNum *2);
		//++CurrentCol;
		if (CurrentCol >= AnimColNum)
		{
			switch (_Dir)
			{
			case math::EDir::left:
				break;
			case math::EDir::right:
				break;
			case math::EDir::up:
				AnimRowIndex++;
				break;
			case math::EDir::down:
				AnimRowIndex++;
				break;
			default:
				break;
			}
			CurrentCol = 0;
		}
	}

	return _event;
}

void SwordManAttack::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!bRender)return;

	object::render(hdc, camera_pos, size_factor);

	if (!EffectImg)return;

	//월드 사이즈
	int wsx = (PaintSizeX * ScaleX);
	int wsy = (PaintSizeY * ScaleY);

	vec  v = { ImgLocationX,ImgLocationY };
	v -= camera_pos;

	GdiTransparentBlt(hdc, v.x - wsx / 2, v.y - wsy / 2,
		wsx, wsy, EffectImg->Get_MemDC(), PaintSizeX * CurrentCol, PaintSizeY * AnimRowIndex
		, PaintSizeX, PaintSizeY, ColorKey);
};

void SwordManAttack::initialize()
{
	object::initialize();
	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EMonsterAttack, ECircle);
	auto sp_comp = _collision_component.lock();
	if (!sp_comp)return;
	sp_comp->bCollision = false;
	sp_comp->bCollisionSlideAnObject= false;
	sp_comp->bCollisionTargetPushFromForce = false;
	sp_comp->bSlide = false;
	sp_comp->HitColor = RGB(123, 123, 123);
	sp_comp->PushForce = 0.01f;
	sp_comp->_size = { 80.f,80.f };
	sp_comp->bRender = true;
	bAttacking = true;
	CurrentAttackDuration = DefaultAttackDuration = 0.2f;

	ObjectTag = object::Tag::monster_attack;
	bInvalidatedefense = true;
}

void SwordManAttack::Hit(std::weak_ptr<object> _target)
{
	object::Hit(_target);

	auto sp_target = _target.lock();
	if (!sp_target)return;
	if (!sp_target->_transform)return;

	if (sp_target->ObjectTag == object::Tag::player_shield)
	{
		if (CurrentAttackDuration > 0)
		{
			shield::DefenseMsg(sp_target->_transform->_location);
			Attack = { 0,0 };
			CurrentAttackDuration = -1;
			auto sp_comp = _collision_component.lock();
			if (!sp_comp)return;
			sp_comp->bCollision = false;
		}
	}
}

