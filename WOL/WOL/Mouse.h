#pragma once
#include "UI.h"

class Mouse :public UI
{
public:
	void initialize()override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;

	Event update(float dt)override;

	uint32_t get_layer_id()const& override;

	std::weak_ptr<class collision_component> _collision_component{};
};

