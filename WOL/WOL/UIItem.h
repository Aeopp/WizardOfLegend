#pragma once
#include "UIInteraction.h"

class UIItem :
    public UIInteraction
{
public:
	void initialize()override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void OnMouse(const vec hit_pos)override;
	void Click(const vec hit_pos)override;
	Event update(float dt)override;
	uint32_t get_layer_id() const&;
	void SetUp(vec location, std::wstring ImgKey, vec PaintSize, std::function<bool()> Event);

	std::function<bool()> _Event;
};

