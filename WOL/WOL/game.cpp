#include "pch.h"
#include "game.h"
#include "object_mgr.h"
#include "collision_mgr.h"
#include "Input_mgr.h"

void game::render(HDC hdc)
{
	object_mgr::instance().render(hdc);

	collision_mgr::instance().render(hdc);
}

void game::update()
{
	object_mgr::instance().update();

	collision_mgr::instance().update();

	Input_mgr::instance().update();
}

void game::initialize()
{
	object_mgr::instance().initialize();

	collision_mgr::instance().initialize();

	Input_mgr::instance().initialize();
}

void game::release()
{
	collision_mgr::instance().release();

	object_mgr::instance().release();

	Input_mgr::instance().release();
}
