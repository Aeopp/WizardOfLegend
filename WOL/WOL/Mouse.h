#pragma once
#include "UI.h"

class Mouse :
    public UI
{
public:
	void initialize()override;
	void update(float dt)override;
	void render(HDC hdc, vec camera_pos)override;

	uint32_t get_layer_id()const& override;

	std::shared_ptr<class collision_component> _collision_component{};
};

