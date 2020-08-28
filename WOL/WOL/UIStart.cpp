#include "pch.h"
#include "UIStart.h"
#include "render_component.h"
#include "game.h"
#include "Color.h"
#include "player_info.h"
#include "Font.h"
#include "Scene_mgr.h"
#include "Input_mgr.h"
#include "Bmp_mgr.h"
#include "game.h"



void UIStart::initialize()
{
	UIInteraction::initialize();

	// 해상도 변경시 비율 조정을 위해서
	auto [x, y] = game::instance().size_factor();

	wp_bmp_main =Bmp_mgr::instance().Insert_Bmp(L"MAIN_MENU.bmp", L"MAIN");
	wp_bmp_quit =Bmp_mgr::instance().Insert_Bmp(L"QUIT_MENU.bmp", L"QUIT");
	wp_bmp_option = Bmp_mgr::instance().Insert_Bmp(L"OPTION_MENU.bmp", L"OPTION");
	wp_bmp_ready= Bmp_mgr::instance().Insert_Bmp(L"READY_MENU.bmp", L"READY_MENU");

	_render_component = std::make_shared<render_component>();
	_render_component->Default_Dest_Paint_Size = vec{ 1600 * x,900* y };
	_render_component->_ColorKey = COLOR::MEGENTA();
	_render_component->_Img_src = RECT{ 0,0,1920,1080};
	_render_component->_RenderDesc = ERender::Transparent;

	_transform->_location = vec{ 0,0 };
	_transform->_size = vec{ 1600,900 };

	eMenu = Ready;

}

void UIStart::render(HDC hdc, vec camera_pos, vec size_factor)
{
	object::render(hdc, camera_pos, size_factor);

	vec loc = _transform->_location;
	vec ds = _render_component->Default_Dest_Paint_Size;

	_render_component->Dest_Loc = loc;
	_render_component->Dest_Paint_Size = ds;

	_render_component->Render(hdc);


	Input_mgr& _Input = Input_mgr::instance();
	auto omv = _Input.GetWindowMousePos();

	POINT pt = make_pt(omv->x, omv->y);
	
	std::wstringstream wss;
	wss << L"X : " << pt.x << L" Y : " << pt.y << std::endl;

	TextOut(hdc, pt.x, pt.y, wss.str().c_str(), wss.str().size());

}

void UIStart::OnMouse(const vec hit_pos)
{
}

void UIStart::Click(vec hit_pos)
{
	switch (eMenu)
	{
	case UIStart::Ready:
		bStart = true;
		eMenu = Main;
		break;
	case UIStart::Main:
		Scene_mgr::instance().Scene_Change(ESceneID::EStage);
		break;
	case UIStart::Option:
		Scene_mgr::instance().Scene_Change(ESceneID::EEdit);
		// 옵션 진입
		break;
	case UIStart::Quit:
		DestroyWindow(game::instance().hWnd);
		break;
	case UIStart::Max:
		break;
	default:
		break;
	}

}

Event UIStart::update(float dt)
{
	UIInteraction::update(dt);

	switch (eMenu)
	{
	case UIStart::Ready:
		_render_component->wp_Image = wp_bmp_ready;
		break;
	case UIStart::Main:
		_render_component->wp_Image = wp_bmp_main;
		break;
	case UIStart::Option:
		_render_component->wp_Image = wp_bmp_option;
		break;
	case UIStart::Quit:
		_render_component->wp_Image = wp_bmp_quit;
		break;
	case UIStart::Max:
		break;
	default:
		break;
	}

	if (!bStart)return Event::None;

	Input_mgr& _Input = Input_mgr::instance();
	auto omv =_Input.GetWindowMousePos();

	POINT pt = make_pt(omv->x,omv->y);

	
	RECT main{ 685,441,907,487 }, opt{ 743,639,848,674 }, quit{ 763,773,837,803 };

	if (PtInRect(&main, pt))
		eMenu = EMenu::Main;
	 if (PtInRect(&opt, pt))
		eMenu = EMenu::Option;
	 if (PtInRect(&quit, pt))
		eMenu = EMenu::Quit;

	return Event::None;
}

uint32_t UIStart::get_layer_id() const&
{
	return layer_type::EUI;
}


