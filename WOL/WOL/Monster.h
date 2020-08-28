#pragma once
#include "actor.h"
#include "collision_component.h"
#include "EnemyInfo.h"

class Monster :
    public actor
{
public:
	void initialize()override;
	Event update(float dt)override;
	void Hit(std::weak_ptr<object> _target)override;
	virtual void Attack();
	void late_initialize(std::weak_ptr<class object> SetTarget);
public:
	std::weak_ptr<collision_component>  _collision_component{};
	std::weak_ptr<class object> _AttackTarget;
	std::shared_ptr<EnemyInfo> _EnemyInfo{};
};

