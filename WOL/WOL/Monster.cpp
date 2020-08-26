#include "pch.h"
#include "Monster.h"
#include "collision_mgr.h"

void Monster::initialize()
{
	object::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EMonster, ERect);
	auto sp_collision = _collision_component.lock();

	if (!sp_collision)return;

	sp_collision->_size = { 50.0f,50.0f };

	_transform->_location = { 300.f,300.f };
};

Event Monster::update(float dt)
{
	Event _E = object::update(dt);
	return _E;
}

