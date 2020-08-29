#include "pch.h"
#include "UIHPBar.h"

#include "game.h"
#include "render_component.h"
#include "Color.h"
#include "player_info.h"

void UIHPBar::initialize()
{
	UI::initialize();

	// 해상도 변경시 비율 조정을 위해서
	auto [x, y] = game::instance().size_factor();

	_render_component = render_component::LoadRenderComponent_SP(L"UI_HPBAR.bmp", L"UI_HPBAR");
	_render_component->Default_Src_Paint_Size = vec{ 166* x,25 * y };
	_render_component->_ColorKey = COLOR::MEGENTA();
	_render_component->_Img_src = RECT{ 0,0,244,32 };
	_render_component->_RenderDesc = ERender::Transparent;

	_transform->_location = vec{ 181,72 };

	this->goal_time = 0.5f;
};

Event UIHPBar::update(float dt)
{
	auto sp_info = wp_info.lock();
	if (sp_info)
	{
		target = sp_info->hp;
	}

	return UIBar::update(dt);;
}

void UIHPBar::render(HDC hdc, vec camera_pos, vec size_factor)
{
	UIBar::render(hdc, camera_pos, size_factor);
}

uint32_t UIHPBar::get_layer_id() const&
{
	return UIBar::get_layer_id();
}
