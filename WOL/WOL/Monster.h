#pragma once
#include "actor.h"
#include "collision_component.h"

class Monster :
    public actor
{
public:
	std::shared_ptr<collision_component> _collision_component;

	void initialize()override;
	Event update(float dt)override;
	void render(HDC hdc,vec camera_pos)override;

	
};

