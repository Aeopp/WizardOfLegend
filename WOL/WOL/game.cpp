#include "pch.h"
#include "game.h"
#include "object_mgr.h"
#include "collision_mgr.h"
#include "Input_mgr.h"
#include "timer.h"
#include "sound_mgr.h"
#include "Bmp_mgr.h"

void game::render(HDC hdc)
{
	Timer::instance().render(hdc);

	object_mgr::instance().render(hdc);

	collision_mgr::instance().render(hdc);
}

void game::update()
{
	Timer::instance().update();

	sound_mgr::instance().update();

	object_mgr::instance().update();

	collision_mgr::instance().update();
}

void game::initialize()
{
	Timer::instance().initialize();

	sound_mgr::instance().initialize();

	Bmp_mgr::instance().initialize();


	Input_mgr::instance().initialize();

	object_mgr::instance().initialize();

	collision_mgr::instance().initialize();
}

void game::release()
{
	Timer::instance().release();

	sound_mgr::instance().release();

	Bmp_mgr::instance().release();

	collision_mgr::instance().release();

	object_mgr::instance().release();

	Input_mgr::instance().release();
}

void game::late_update()
{
	Input_mgr::instance().update();
}
