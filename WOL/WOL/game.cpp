#include "pch.h"
#include "game.h"
#include "object_mgr.h"
#include "collision_mgr.h"
#include "Input_mgr.h"
#include "timer.h"
#include "sound_mgr.h"
#include "Bmp_mgr.h"
#include "Font.h"

void game::render(HDC hdc)
{
	{
		auto FontOn = Font(hdc, L"", 20, RGB(255, 0, 0));

		Timer::instance().render(hdc);

		object_mgr::instance().render(hdc, size_factor());

		collision_mgr::instance().render(hdc, size_factor());
	}
}

void game::update()
{
	Timer& _Timer = Timer::instance();

	_Timer.update();

	sound_mgr::instance().Frame(_Timer.delta());

	object_mgr::instance().update();

	collision_mgr::instance().update();
}

void game::initialize()
{
	AddFontResource(L"..\\..\\Resources\\Font\\NanumBarunGothicBold.ttf");

	Timer::instance().initialize();

	sound_mgr::instance().initialize();

	Bmp_mgr::instance().initialize();

	Input_mgr::instance().initialize();

	object_mgr::instance().initialize();
}

void game::release()
{
	Bmp_mgr::instance().release();

	object_mgr::instance().release();
}

void game::late_update()
{
	Input_mgr::instance().update();
}

std::pair<float, float> game::size_factor()
{
	return {
		(float)(client_rect.right - client_rect.left) / width  ,
		 (float)(client_rect.bottom - client_rect.top) / height };
}
