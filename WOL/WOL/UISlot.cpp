#include "pch.h"
#include "UISlot.h"
#include "render_component.h"
#include "game.h"
#include "Color.h"
#include "Bmp_mgr.h"
#include "Input_mgr.h"

void UISlot::initialize()
{
	UIInteraction::initialize();

	_render_component = std::make_shared<render_component>();
	_render_component->Default_Src_Paint_Size = vec{ 52,52};
	_render_component->_ColorKey = COLOR::MRGENTA();
	_render_component->_Img_src = RECT{ 0,0,52,52 };
	_render_component->_RenderDesc = ERender::Transparent;

	_transform->_location = vec{ 0,0 };
	Scale = 0.85f;
	_transform->_size = vec{ 52 * Scale,52* Scale };
}

void UISlot::render(HDC hdc, vec camera_pos, vec size_factor)
{
	UIInteraction::render(hdc, camera_pos, size_factor);
}


void UISlot::OnMouse(const vec hit_pos)
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		Click(*Input_mgr::instance().GetWindowMousePos());
	}
}

void UISlot::Click(const vec hit_pos)
{
	
}

Event UISlot::update(float dt)
{
	return UIInteraction::update(dt);
}

uint32_t UISlot::get_layer_id() const&
{
    return layer_type::EUIIcon;
}

void UISlot::SetUp(vec location, std::wstring ImgKey)
{
	if (!_transform)return;
	_transform->_location = std::move(location);
	_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(ImgKey); 
}
