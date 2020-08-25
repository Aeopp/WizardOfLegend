#include "pch.h"
#include "Mouse.h"
#include "collision_mgr.h"
#include "game.h"
#include "Input_mgr.h"

void Mouse::render(HDC hdc, vec camera_pos)
{
	UI::render(hdc, camera_pos);
};

void Mouse::initialize()
{
	UI::initialize();

	_transform->_size = { 10,10 };

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EMouse, ECircle);
	if (!_collision_component)return;

	_collision_component->_size = { 10.f,10.0f };
	_collision_component->bPush = false;
	_collision_component->bRender = false;

	ShowCursor(true);

	Input_mgr::instance()._Mouse = _ptr;
}

Event Mouse::update(float dt)
{
	Event _E = UI::update(dt);

	POINT		pt = {};
	GetCursorPos(&pt);
	ScreenToClient(game::instance().hWnd, &pt);

	_transform->_location = vec{ pt.x,pt.y };

	return _E; 
};

uint32_t Mouse::get_layer_id() const&
{
	return layer_type::EUI;
};

