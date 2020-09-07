#include "pch.h"
#include "UIInteraction.h"
#include "Input_mgr.h"
#include "sound_mgr.h"

Event UIInteraction::update(float dt)
{
	UI::update(dt);

	Input_mgr& _Input_mgr = Input_mgr::instance();
	auto pos = _Input_mgr.GetWindowMousePos();
	Input_mgr::instance()._Mouse;

	if (!pos) return Event::None;

	vec v = _transform->_location;
	vec s = _transform->_size;

	RECT rt = make_rect(v.x, v.y, v.x + s.x, v.y + s.y);

	bool bClick = _Input_mgr.Key_Down(VK_LBUTTON);

	if (PtInRect(&rt, make_pt(pos->x, pos->y)))
	{
		bOnMouse = true;
		if (bClick)
		{
		//	SOUNDPLAY("CLICK_MENU", 1.f, false);
			Click(*pos);
		}
		else
		{
			OnMouse(*pos);
		}
	}
	else
	{
		bOnMouse = false;
	}

	return Event::None;
}
