#include "pch.h"
#include "UIBar.h"
#include "render_component.h"
#include "Debuger.h"
#include "player_info.h"

Event UIBar::update(float dt)
{
	UI::update(dt);

	current = math::my_lerp(current, target, goal_time, dt);

	ratio = current / max;
	return Event::None;
};

void UIBar::render(HDC hdc, vec camera_pos, vec size_factor)
{
	object::render(hdc, camera_pos, size_factor);

	vec loc = _transform->_location;
	vec s = _transform->_size;
	vec ps = _render_component->Dest_Paint_Size;
	vec ds = _render_component->Default_Dest_Paint_Size;

	s.x *= size_factor.x;
	s.y *= size_factor.y;

	Debuger(hdc, [&] {Rectangle(hdc, loc.x - s.x, loc.y - s.y, loc.x + s.x, loc.y + s.y); });

	if (!_render_component)return;

	ps.x *= size_factor.x;
	ps.y *= size_factor.y;

	loc.y *= size_factor.y;
	loc.x *= size_factor.x;

	loc -= _render_component->Default_Dest_Paint_Size * 0.5f;

    _render_component->Dest_Loc = loc;
	_render_component->Dest_Paint_Size.x = (ds.x * size_factor.x)*ratio;
	_render_component->Dest_Paint_Size.y = ds.y * size_factor.y;
	_render_component->Render(hdc);
}
uint32_t UIBar::get_layer_id() const&
{
	return layer_type::EUIInfo;
};



