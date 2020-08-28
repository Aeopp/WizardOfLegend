#include "pch.h"
#include "game.h"
#include "Font.h"
#include "Scene_mgr.h"
#include "timer.h"
#include "sound_mgr.h"
#include "Bmp_mgr.h"
#include "object_mgr.h"
#include "collision_mgr.h"
#include "player_info.h"



void game::render(HDC hdc)
{
	Scene_mgr::instance().render(hdc,size_factor());

	if (bDebug)
	{
		std::wstringstream wss;
		wss << L"컬링 오브젝트 : " << print_cul_obj << L" 렌더링 오브젝트 " << print_render_obj << std::endl;
		TextOut(hdc, 1000, 0, wss.str().c_str(), wss.str().size());
	}
}

void game::update()
{
	Scene_mgr::instance().update(Timer::instance().delta());
}

void game::initialize()
{
	_player_info = std::make_shared<player_info>();

	Scene_mgr::instance().initialize();

	Scene_mgr::instance().Scene_Change(ESceneID::EStart);
}

void game::release()
{
	Scene_mgr::instance().release();

	Bmp_mgr::instance().release();

	sound_mgr::instance().Release();

	object_mgr::instance().release();

	collision_mgr::instance().release();
}

void game::late_update()
{

}

void game::debug_cul_obj_setup(int cul_obj, int render_obj)
{
	print_cul_obj = cul_obj;
	print_render_obj = render_obj;
}


std::pair<float, float> game::size_factor()
{
	return {
		(float)(client_rect.right - client_rect.left) / width  ,
		 (float)(client_rect.bottom - client_rect.top) / height };
}
