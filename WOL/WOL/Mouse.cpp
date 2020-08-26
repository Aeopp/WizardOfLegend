#include "pch.h"
#include "Mouse.h"
#include "collision_mgr.h"
#include "game.h"
#include "Input_mgr.h"
#include "render_component.h"

void Mouse::render(HDC hdc, vec camera_pos)
{
	UI::render(hdc, camera_pos);

	if (!_render_component)return;

	vec _loc = _transform->_location;
	RECT src_rt = _render_component->_Img_src;
	vec _size = vec{ src_rt.right - src_rt.left, src_rt.bottom - src_rt.top };

	_render_component->_Img_Dest = 
		make_rect(_loc.x - _size.x, _loc.y - _size.y,
		_size.x + _size.x, _size.y + _size.y);

	_render_component->Render(hdc);
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

	ShowCursor(false);

	Input_mgr::instance()._Mouse = _ptr;

	_render_component = render_component::MakeRenderComponent_SP(L"UI_MOUSE.bmp", L"UI_MOUSE");
	_render_component->_ColorKey = RGB(255, 0, 255);
	_render_component->_Img_src = RECT{0,0,60,60};
	_render_component->_RenderDesc = ERender::Transparent;
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

