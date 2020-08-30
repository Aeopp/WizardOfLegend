#pragma once
#include "actor.h"
class ICE_Blast :
    public actor
{
public:
	void initialize()override;
	//void late_initialize(vec _location);
	Event update(float dt)override;
	std::weak_ptr<class collision_component>  _collision_component;

	uint32_t get_layer_id()const& override;

	vec _target{};

	float Minimum_distance{};
	bool bSuccess{};
};

