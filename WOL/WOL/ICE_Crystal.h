#pragma once
#include "actor.h"
class ICE_Crystal :
    public actor
{
public:
	void initialize()override;
	Event update(float dt)override;
	std::weak_ptr<class collision_component>  _collision_component;

	uint32_t get_layer_id()const& override;

	vec _target{};

	float Minimum_distance{};
	bool bSuccess{};
};

