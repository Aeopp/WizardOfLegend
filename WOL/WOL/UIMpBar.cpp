#include "pch.h"
#include "UIMPBar.h"

#include "game.h"
#include "render_component.h"
#include "Color.h"
#include "player_info.h"

void UIMPBar::initialize()
{
	UI::initialize();

	// 해상도 변경시 비율 조정을 위해서
	auto [x, y] = game::instance().size_factor();

	_render_component = render_component::LoadRenderComponent_SP(L"UI_MANABAR.bmp", L"UI_MANABAR");
	_render_component->Default_Src_Paint_Size = vec{ 132* x,16 * y };
	_render_component->_ColorKey = COLOR::MEGENTA();
	_render_component->_Img_src = RECT{ 0,0,192,16 };
	_render_component->_RenderDesc = ERender::Transparent;

	_transform->_location = vec{ 52 + (162-50),50 + ( 92-50) };
	

	this->goal_time = 0.5f;
};

Event UIMPBar::update(float dt)
{
	auto sp_info = wp_info.lock();
	if (sp_info)
	{
		target = sp_info->GetMP();
	}

	return UIBar::update(dt);;
}

void UIMPBar::render(HDC hdc, vec camera_pos, vec size_factor)
{
	UIBar::render(hdc, camera_pos, size_factor);
}

uint32_t UIMPBar::get_layer_id() const&
{
	return UIBar::get_layer_id();
};

