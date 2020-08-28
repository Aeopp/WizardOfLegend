#include "pch.h"
#include "Monster.h"
#include "collision_mgr.h"

void Monster::initialize()
{
	actor::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EMonster, ERect);
	auto sp_collision = _collision_component.lock();

	if (!sp_collision)return;

	sp_collision->_size = { 50.0f,50.0f };

	_transform->_location = { 300.f,300.f };

	_EnemyInfo = std::make_shared<EnemyInfo>();
};

Event Monster::update(float dt)
{
	Event _E = actor::update(dt);
	return _E;
}

void Monster::Hit(std::weak_ptr<object> _target)
{
	actor::Hit(_target);


}

void Monster::Attack()
{
}

void Monster::late_initialize(std::weak_ptr<class object> SetTarget)
{
	_AttackTarget = std::move(SetTarget);
}

