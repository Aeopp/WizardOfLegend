#include "pch.h"
#include "UISkillIBarIcon.h"

#include "game.h"
#include "render_component.h"
#include "Color.h"

void UISkillIBarIcon::late_initialize(vec pos,std::wstring FileName)
{
	// 해상도 변경시 비율 조정을 위해서
	auto [x, y] = game::instance().size_factor();

	_render_component = render_component::LoadRenderComponent_SP(FileName.c_str(), FileName.c_str());
	_render_component->Default_Src_Paint_Size = vec{ 40 * x,40 * y };
	_render_component->_ColorKey = COLOR::MEGENTA();
	_render_component->_Img_src = RECT{ 0,0,54,54 };
	_render_component->_RenderDesc = ERender::Transparent;

	_transform->_location = std::move(pos); 

}
