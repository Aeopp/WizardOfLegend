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
#include "Bmp.h"

void game::render()
{
	Bmp_mgr& bmp_mgr =Bmp_mgr::instance();

	auto SP_Back =bmp_mgr.Find_Image_SP(L"Back");
	if (!SP_Back)return;

	auto SP_BackBuffer = bmp_mgr.Find_Image_SP(L"BackBuffer");
	if (!SP_BackBuffer)return;

	static HDC HMemDC; HDC HBackBuffer;

	HMemDC = SP_Back->Get_MemDC();
	HBackBuffer = SP_BackBuffer->Get_MemDC();

	BitBlt(HBackBuffer, 0, 0, client_rect.right, client_rect.bottom, HMemDC, 0, 0, SRCCOPY);

	CurrentHdc = HBackBuffer;
	if (bDebug)
	{
		std::wstringstream wss;
		wss << L"컬링 오브젝트 : " << print_cul_obj << L" 렌더링 오브젝트 " << print_render_obj << std::endl;
		TextOut(CurrentHdc, 1400, 0, wss.str().c_str(), wss.str().size());
	}

	Scene_mgr::instance().render(HBackBuffer, size_factor());

	BitBlt(hDC, 0, 0, client_rect.right, client_rect.bottom, HBackBuffer, 0, 0, SRCCOPY);

	CurrentHdc = hDC;
}

void game::update()
{
	Scene_mgr::instance().update(Timer::instance().delta());
}

void game::initialize()
{
	hDC = GetDC(hWnd);

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

	ReleaseDC(hWnd, CurrentHdc);
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
