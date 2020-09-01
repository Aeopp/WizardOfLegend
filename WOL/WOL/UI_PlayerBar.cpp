#include "pch.h"
#include "UI_PlayerBar.h"

#include "Font.h"
#include "game.h"
#include "render_component.h"
#include "Color.h"
#include "player_info.h"

void UI_PlayerBar::initialize()
{
	UI::initialize();

	// 해상도 변경시 비율 조정을 위해서
	auto [x, y] = game::instance().size_factor();

	_render_component = render_component::LoadRenderComponent_SP(L"UI_PLAYERBAR.bmp", L"UI_PLAYERBAR");
	_render_component->Default_Src_Paint_Size = vec{ 328/1.5* x,80/1.5* y };
	_render_component->_ColorKey = COLOR::MEGENTA();
	_render_component->_Img_src = RECT{ 0,0,328,80 };
	_render_component->_RenderDesc = ERender::Transparent;

	_transform->_location = vec{ 160,80 };
};


void UI_PlayerBar::render(HDC hdc, vec camera_pos, vec size_factor)
{
	UI::render(hdc, camera_pos, size_factor);

	auto sp_info = _player_info.lock();

	if (!sp_info)return;

	vec loc = _transform->_location;

	loc.y *= size_factor.y;
	loc.x *= size_factor.x;

	loc -= _render_component->Dest_Paint_Size * 0.5f;

	
	{
		std::wstringstream wss;
		wss << (int)sp_info->GetHP()<< L"/" << (int)sp_info->max_hp << std::endl;
		Font(hdc, wss.str(), 20, RGB(150, 0, 0), (int)loc.x + 100, (int)loc.y - 35);
	}
	
		{
			std::wstringstream wss;
			wss << (int)sp_info->GetMP()<< L"/" << (int)sp_info->max_mp << std::endl;
			Font(hdc, wss.str(), 20, RGB(0, 0, 150), (int)loc.x + 100, (int)loc.y - 13);
			Font(hdc, L"", 20, RGB(255, 0, 0));
		}
		
	
		
}
Event UI_PlayerBar::update(float dt)
{
	return 	UI::update(dt);
}
void UI_PlayerBar::late_initialize()
{
	
};






