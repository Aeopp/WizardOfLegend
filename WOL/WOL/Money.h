#pragma once
#include "Effect.h"
class Money :
    public Effect
{
public:
	Event update(float dt)override;
	void initialize()override;
	uint32_t get_layer_id()const& override;
};

