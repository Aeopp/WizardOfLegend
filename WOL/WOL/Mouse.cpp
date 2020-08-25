#include "pch.h"
#include "Mouse.h"
#include "collision_mgr.h"
#include "game.h"

void Mouse::render(HDC hdc, vec camera_pos)
{
	vec loc = _transform->_location;
	vec s = _transform->_size;

	Ellipse(hdc, loc.x - s.x, loc.y - s.y, loc.x + s.x, loc.y + s.y);
};

void Mouse::initialize()
{
	object::initialize();

	_transform->_size = { 10,10 };

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::mouse, ECircle);
	if (!_collision_component)return;

	_collision_component->_size = { 10.f,10.0f };
	_collision_component->bPush = false;
	_collision_component->bRender = false;
}

void Mouse::update(float dt)
{
	object::update(dt);

	POINT		pt = {};
	GetCursorPos(&pt);
	ScreenToClient(game::instance().hWnd, &pt);

	ShowCursor(true);

	_transform->_location = vec{ pt.x,pt.y };

	return;
};

uint32_t Mouse::get_layer_id() const&
{
	return layer_type::EUI;
};

