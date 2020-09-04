#include "pch.h"
#include "UIBossHPBar.h"
#include "game.h"
#include "render_component.h"
#include "Color.h"
#include "player_info.h"
#include "BossInfo.h"

void UIBossHPBar::initialize()
{
	UI::initialize();

	// 해상도 변경시 비율 조정을 위해서
	_render_component = render_component::LoadRenderComponent_SP(L"BOSS_HPBAR.bmp", L"BOSS_HPBAR");
	_render_component->Default_Src_Paint_Size = vec{ 356 ,28 };
	_render_component->_ColorKey = COLOR::MEGENTA();
	_render_component->_Img_src = RECT{ 0,0,356,28 };
	_render_component->_RenderDesc = ERender::Transparent;

	_transform->_location = vec{ 500,92 };

	this->goal_time = 1.f;
};

Event UIBossHPBar::update(float dt)
{

	return UIBar::update(dt);;
}

void UIBossHPBar::render(HDC hdc, vec camera_pos, vec size_factor)
{
	UIBar::render(hdc, camera_pos, size_factor);
}

uint32_t UIBossHPBar::get_layer_id() const&
{
	return UIBar::get_layer_id();
};



