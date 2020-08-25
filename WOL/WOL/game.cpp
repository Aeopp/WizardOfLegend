#include "pch.h"
#include "game.h"
#include "object_mgr.h"
#include "collision_mgr.h"
#include "Input_mgr.h"
#include "timer.h"

void game::render(HDC hdc)
{
	Timer::instance().render(hdc);

	object_mgr::instance().render(hdc);

	collision_mgr::instance().render(hdc);
}

void game::update()
{
	Timer::instance().update();

	object_mgr::instance().update();

	collision_mgr::instance().update();

	Input_mgr::instance().update();
}

void game::initialize()
{
	Timer::instance().initialize();

	Input_mgr::instance().initialize();

	object_mgr::instance().initialize();

	collision_mgr::instance().initialize();
}

void game::release()
{
	Timer::instance().release();

	collision_mgr::instance().release();

	object_mgr::instance().release();

	Input_mgr::instance().release();
}
