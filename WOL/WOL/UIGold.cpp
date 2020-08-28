#include "pch.h"
#include "UIGold.h"
#include "render_component.h"
#include "game.h"
#include "Color.h"
#include "player_info.h"
#include "Font.h"

void UIGold::initialize()
{
	UI::initialize();

	// 해상도 변경시 비율 조정을 위해서
	auto [x, y] = game::instance().size_factor();

	_render_component = render_component::LoadRenderComponent_SP(L"UI_MONEY.bmp", L"UI_MONEY");
	_render_component->Default_Dest_Paint_Size = vec{ 31 * x,31 * y };
	_render_component->_ColorKey = COLOR::MEGENTA();
	_render_component->_Img_src = RECT{ 0,0,31,31 };
	_render_component->_RenderDesc = ERender::Transparent;

	_transform->_location = vec{ 788,830 };
}

void UIGold::render(HDC hdc, vec camera_pos, vec size_factor)
{
	UI::render(hdc, camera_pos, size_factor);
	auto sp_info = _player_info.lock();

	if (!sp_info)return;

	vec loc = _transform->_location;

	loc.y *= size_factor.y;
	loc.x *= size_factor.x;

	loc -= _render_component->Dest_Paint_Size * 0.5f;

	{
		std::wstring str = std::to_wstring((int)sp_info->gold);
		RECT _rt = { loc.x + (36* size_factor.x),(loc.y-3) + (0* size_factor.y),loc.x + (76 * size_factor.x),(loc.y-3) + (39 * size_factor.y) };
		Font(hdc, str, (_rt.right - _rt.left), RGB(200, 200, 200), _rt.left, _rt.top);

		Font(hdc, L"");
		//DrawText(hdc, str.c_str(), str.size(), &_rt, DT_CENTER);
	}
}

uint32_t UIGold::get_layer_id() const&
{
	return layer_type::EUI;
}
