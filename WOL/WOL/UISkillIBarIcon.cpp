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
	_render_component->_ColorKey = COLOR::MEGENTA();
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
	auto& r = _render_component;

	vec v = r->Dest_Loc = _transform->_location;
	v.x -= r->Default_Src_Paint_Size.x / 2;
	v.y -= r->Default_Src_Paint_Size.y / 2;

	vec s = {r->_Img_src.right, r->_Img_src.bottom};


	GdiTransparentBlt(hdc, v.x, v.y, r->Default_Src_Paint_Size.x,
	r->Default_Src_Paint_Size.y,
		r->wp_Image.lock()->Get_MemDC(), 0, 0, s.x, s.y, RGB(255, 0, 255));
}

Event UISkillIBarIcon::update(float dt)
{
	return UIInfo::update(dt);
}
