#include "pch.h"
#include "UISkillIBarIcon.h"

#include "Bmp.h"

#include "game.h"
#include "render_component.h"
#include "Color.h"

void UISkillIBarIcon::late_initialize(vec pos,std::wstring FileName)
{
	// 해상도 변경시 비율 조정을 위해서
	auto [x, y] = game::instance().size_factor();

	_render_component = render_component::LoadRenderComponent_SP(FileName.c_str(), FileName.c_str());
	_render_component->Default_Src_Paint_Size = vec{ 40 * x,40 * y };
	_render_component->_ColorKey = COLOR::MRGENTA();
	_render_component->_Img_src = RECT{ 0,0,54,54 };
	_render_component->_RenderDesc = ERender::Transparent;

	_transform->_location = std::move(pos); 

}

void UISkillIBarIcon::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!Max)return;
	if (!Current)return;

	float fCurrent = *Current;
	float fMax = *Max;

	float factor = fCurrent / fMax;
	auto& Render = _render_component;

	vec v = Render->Dest_Loc = _transform->_location;
	v.x -= Render->Default_Src_Paint_Size.x / 2;
	v.y -= Render->Default_Src_Paint_Size.y / 2;

	vec WorldSize = Render->Default_Src_Paint_Size;

	vec ImgSize = {Render->_Img_src.right, Render->_Img_src.bottom};

	float re = 1 - factor;

	GdiTransparentBlt(hdc, v.x, v.y+WorldSize.y*re, Render->Default_Src_Paint_Size.x,
	/*r->Default_Src_Paint_Size.y*/0 + WorldSize.y * factor,
		Render->wp_Image.lock()->Get_MemDC(), 0, 0+ImgSize.y*re, ImgSize.x, /*s.y*/0 + ImgSize.y*factor, RGB(255, 0, 255));
}

Event UISkillIBarIcon::update(float dt)
{
	return UIInfo::update(dt);
}
