#include "pch.h"

#include "ArcherArrow.h"
#include "collision_component.h"
#include "collision_mgr.h"
#include "Bmp.h"
#include "shield.h"
#include "shield.h"

void ArcherArrow::launch(vec init,vec dir, int Row, std::shared_ptr<class Bmp> SetImg)
{
	if (!_transform)return;
	this->RowIdx = Row;
	_transform->_location = std::move(init);
	_transform->_dir = std::move(dir);
	sp_Img = std::move(SetImg);

	auto sp_comp = wp_collision.lock();
	if (!sp_comp)return;
	sp_comp->bPush = true;
	sp_comp->bObjectSlide = false;
	sp_comp->bHitEffect = true;
	sp_comp->bCollision= true;
}

void ArcherArrow::render(HDC hdc, vec camera_pos, vec size_factor)
{
	vec v = _transform->_location - camera_pos;
	v.x -= (PaintSizeX / 2);
	v.y -= (PaintSizeY / 2);
	
 	if (!sp_Img)return;

	GdiTransparentBlt(hdc, v.x, v.y, PaintSizeX, PaintSizeY,
		sp_Img->Get_MemDC(), 4 * PaintSizeX, RowIdx * PaintSizeY,
		PaintSizeX, PaintSizeY, RGB(0, 255, 255));
}

Event ArcherArrow::update(float dt)
{
	Event _Event = object::update(dt); 
	
	if (!_transform)return Event::Die;

	_transform->_location += _transform->_dir * dt * speed;
	return _Event ;
}

void ArcherArrow::HitTile(RECT rt)
{
	object::HitTile(rt);

	bDie = true;
}


void ArcherArrow::initialize()
{
	object::initialize();

	wp_collision =
	collision_mgr::instance().insert(_ptr, collision_tag::EMonsterAttack, ECircle);

	auto sp_comp = wp_collision.lock();
	if (!sp_comp)return;

	sp_comp->bHitEffect = false;
	sp_comp->bCollision = false;
	sp_comp->bPush = false;
	sp_comp->bObjectSlide = false;
	sp_comp->HitColor = RGB(123, 200, 50);
	sp_comp->PushForce = 1.f;
	sp_comp->_size = { 13.f,13.f };

	PaintSizeX = 180;
	PaintSizeY = 170;
	ScaleX = 1;
	ScaleY = 1;
	speed = 1100.f;
	id = object::ID::monster_attack;

	Attack = math::Rand<int>({ 20, 40 });
}

uint32_t ArcherArrow::get_layer_id() const&
{
	return layer_type::EObject;
}

void ArcherArrow::Hit(std::weak_ptr<object> _target)
{
	object::Hit(_target);

	/*auto sp_Target =_target.lock();
	auto sp_IsShield = std::dynamic_pointer_cast<shield>(sp_Target);
	if (sp_IsShield)
	{
		bDie = true;
	}*/
};

