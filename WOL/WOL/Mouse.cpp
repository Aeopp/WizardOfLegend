#include "pch.h"
#include "Mouse.h"
#include "collision_mgr.h"
#include "game.h"
#include "Input_mgr.h"
#include "render_component.h"
#include "Debuger.h"

void Mouse::render(HDC hdc, vec camera_pos, vec size_factor)
{
	object::render(hdc, camera_pos, size_factor);

	vec loc = _transform->_location;
	vec s = _transform->_size;
	vec ps = _render_component->Dest_Paint_Size;

	s.x *= size_factor.x;
	s.y *= size_factor.y;

	Debuger(hdc, [&] {Rectangle(hdc, loc.x - s.x, loc.y - s.y, loc.x + s.x, loc.y + s.y); });

	if (!_render_component)return;

	loc -= _render_component->Dest_Paint_Size * 0.5f;
	
	_render_component->Dest_Loc = loc;
	_render_component->Dest_Paint_Size = _render_component->Default_Src_Paint_Size * 1.3f;
	_render_component->Render(hdc);

	if (bDebug)
	{
		std::wstringstream wss;
		wss << loc;
		RECT _rt{ loc.x,loc.y,loc.x + 200,loc.y + 100 };
		DrawText(hdc, wss.str().c_str(), wss.str().size(), &_rt, DT_CENTER);
	}
}

void Mouse::initialize()
{
	UI::initialize();

	ShowCursor(false);

	_transform->_size = { 10,10 };

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EMouse, ECircle);
	auto sp_collision = _collision_component.lock();

	if (!sp_collision)return;

	sp_collision->_size = { 10.f,10.0f };
	sp_collision->bSlide = false;
	sp_collision->bRender = false;

	Input_mgr::instance()._Mouse = _ptr;

	_render_component = render_component::LoadRenderComponent_SP(L"UI_MOUSE.bmp", L"UI_MOUSE");
	_render_component->_ColorKey = RGB(255, 0, 255);
	_render_component->_Img_src = RECT{0,0,60,60};
	_render_component->Default_Src_Paint_Size = vec{ 30,30 };
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
	return layer_type::EUIMouse;
};

