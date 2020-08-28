#include "pch.h"
#include "UISkillBar.h"
#include "render_component.h"
#include "Color.h"
#include "game.h"

void UISkillBar::initialize()
{
	UI::initialize();

	// 해상도 변경시 비율 조정을 위해서
	auto [x, y] = game::instance().size_factor();

	_render_component = render_component::LoadRenderComponent_SP(L"UI_SKILLBAR.bmp", L"UI_SKILLBAR");
	_render_component->Default_Dest_Paint_Size = vec{480*x,120 *y};
	_render_component->_ColorKey = COLOR::MEGENTA();
	_render_component->_Img_src = RECT{0,0,640,160};
	_render_component->_RenderDesc = ERender::Transparent;

	_transform->_location = vec{ 300,830};
}

void UISkillBar::render(HDC hdc, vec camera_pos,vec size_factor)
{

	UI::render(hdc, camera_pos,size_factor);
}

uint32_t UISkillBar::get_layer_id() const&
{
	return UI::get_layer_id();
}
