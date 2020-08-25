#include "pch.h"
#include "Monster.h"
#include "collision_mgr.h"

void Monster::initialize()
{
	object::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EMonster, ERect);
	if (!_collision_component)return;

	_collision_component->_size = { 50.0f,50.0f };

	_transform->_location = { 300.f,300.f };
};

Event Monster::update(float dt)
{
	Event _E = object::update(dt);
	return _E;
}

void Monster::render(HDC hdc,vec camera_pos)
{
	object::render(hdc, camera_pos);
}
