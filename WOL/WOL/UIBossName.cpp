#include "pch.h"
#include "UIBossName.h"
#include "Font.h"
#include "game.h"
#include "render_component.h"
#include "Color.h"

void UIBossName::initialize()
{
	UI::initialize();

};


void UIBossName::render(HDC hdc, vec camera_pos, vec size_factor)
{
	vec loc = _transform->_location;

	loc.y *= size_factor.y;
	loc.x *= size_factor.x;

	loc -= _render_component->Dest_Paint_Size * 0.5f;

	UI::render(hdc, camera_pos, size_factor);
}

Event UIBossName::update(float dt)
{
	if (bDie)
	{
		return Event::Die;
	}

	return object::update(dt);
}
void UIBossName::late_initialize(const std::wstring& UIFileName,const std::wstring& UIKey)
{
	_render_component = render_component::LoadRenderComponent_SP(UIFileName, UIKey);
	_render_component->Default_Src_Paint_Size = vec{ 455,90 };
	_render_component->_ColorKey = COLOR::MRGENTA();
	_render_component->_Img_src = RECT{ 0,0,455,90 };
	_render_component->_RenderDesc = ERender::Transparent;
	_transform->_location = vec{ 800,80 };
};

