#pragma once
#include "actor.h"
class shield :
    public actor
{
public :
	void initialize()override;
	Event update(float dt)override;
	std::weak_ptr<class collision_component>  _collision_component;

	uint32_t get_layer_id()const& override;

	float _shield_distance{ 150.0f };
	float duration{};

};

