#include "pch.h"
#include "WizardFire.h"
#include "object_mgr.h"
#include "collision_component.h"
#include "Camera.h"

#include "collision_mgr.h"
#include "Bmp.h"
#include "sound_mgr.h"
#include "player.h"


void WizardFire::Cast(vec init, vec dir, int Row, std::shared_ptr<class Bmp> SetImg)
{
	if (!_transform)return;
	this->RowIdx = Row;
	_transform->_location = std::move(init);
	_transform->_dir = std::move(dir);
	sp_Img = std::move(SetImg);

	auto sp_comp = wp_collision.lock();
	if (!sp_comp)return;
	sp_comp->bCollisionTargetPushFromForce = true;
	sp_comp->bCollisionSlideAnObject = false;
	sp_comp->bHitEffect = true;
	sp_comp->bCollision = true;
	sp_comp->bTileHitEffect = true;

	bAttacking = true;
}
void WizardFire::render(HDC hdc, vec camera_pos, vec size_factor)
{
	vec v = _transform->_location - camera_pos;
	v.x -= (PaintSizeX / 2);
	v.y -= (PaintSizeY / 2);

	if (!sp_Img)return;

	CalcRowFromAngle(_transform->_dir);

	GdiTransparentBlt(hdc, v.x, v.y, PaintSizeX*ScaleX, PaintSizeY*ScaleY,
		sp_Img->Get_MemDC(), ColIdx * PaintSizeX, RowIdx * PaintSizeY,
		PaintSizeX, PaintSizeY, RGB(110, 70, 210));

	if (bDebug)
	{
		MoveToEx(hdc, _transform->_location.x, _transform->_location.y, nullptr);
		LineTo(hdc, _transform->_dir.x * 100 + _transform->_location.x, _transform->_dir.y * 100 + _transform->_location.y);
	}
}

int WizardFire::CalcRowFromAngle(vec dir)
{
	// 22.5 도마다 스프라이트 준비
	int IDX = math::AngleFromVec(dir) / 22.5f;
	IDX += 8;
	IDX %= 16;

	RowIdx = IDX;

	return 0;
}

Event WizardFire::update(float dt)
{
	Event _Event = object::update(dt);

	if (!_transform)return Event::Die;
	_transform->_location += _transform->_dir * dt * speed;

	CurrentAnimDelta -= dt;
	if (CurrentAnimDelta < 0)
	{
		ColIdx++;
		if (ColIdx >= AnimColNum)
		{
			ColIdx = 0;
		}
		CurrentAnimDelta = AnimBetWeenDelta;
	}

	return _Event;
}

void WizardFire::HitTile(RECT rt)
{
	object::HitTile(rt);

	// 히트 이펙트
	collision_mgr::instance().HitEffectPush(_transform->_location,
		0.5f);

	bDie = true;
}


void WizardFire::initialize()
{
	object::initialize();

	wp_collision =
		collision_mgr::instance().insert
		(_ptr, collision_tag::EMonsterAttack, ECircle);

	auto sp_comp = wp_collision.lock();
	if (!sp_comp)return;

	sp_comp->bHitEffect = true;
	sp_comp->bCollision = true;
	sp_comp->bCollisionTargetPushFromForce = true;
	sp_comp->bCollisionSlideAnObject = false;
	sp_comp->HitColor = RGB(110,70, 210);
	sp_comp->PushForce = 10.f;
	sp_comp->_size = { 13.f,13.f };

	PaintSizeX = 90;
	PaintSizeY = 90;
	ScaleX = 1.f;
	ScaleY = 1.f;
	speed = 200.f;
	ObjectTag = object::Tag::monster_attack;
	ColIdx = 0;
	Attack = { 20,40 };
	// 방어하는 스킬과 상호작용
	bInvalidatedefense = true;

	Attack = { 40,50 };

}

uint32_t WizardFire::get_layer_id() const&
{
	return layer_type::EEffect;
}

void WizardFire::Hit(std::weak_ptr<object> _target)
{
	object::Hit(_target);

	auto sp_target = _target.lock();
	if (!sp_target)return;
	if (!_transform)return;


	if (sp_target->ObjectTag == object::Tag::player_attack)
	{
		bDie = true;
		collision_mgr::instance().HitEffectPush(_transform->_location, 0.5f);
		return;
	}

	if (sp_target->ObjectTag == object::Tag::player)
	{
		bDie = true;
		auto sp_Camera = object_mgr::instance()._Camera.lock();
		if (!sp_Camera)return;
		collision_mgr::instance().HitEffectPush(_transform->_location, 0.5f);
	//	sp_Camera->camera_shake(100.f, math::RandVec(), 10.f);
	}
};


