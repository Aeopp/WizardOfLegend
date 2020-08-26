#pragma once
#include "actor.h"
#include "collision_component.h"

class Monster :
    public actor
{
public:
	std::weak_ptr<collision_component>  _collision_component{};

	void initialize()override;
	Event update(float dt)override;
	
};

