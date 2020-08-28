#include "pch.h"
#include "Scene_Boss.h"
#include "collision_mgr.h"
#include "Scene_mgr.h"
#include "Input_mgr.h"
#include "timer.h"
#include "object_mgr.h"
#include "sound_mgr.h"
#include "Bmp_mgr.h"
#include "Font.h"
Scene_Boss::~Scene_Boss() noexcept
{
	release();

}
void Scene_Boss::render(HDC hdc, std::pair<float, float> size_factor)
{
	{
		auto FontOn = Font(hdc, L"", 20, RGB(255, 0, 0));

		Timer::instance().render(hdc);

		object_mgr::instance().render(hdc, size_factor);
		collision_mgr::instance().render(hdc, size_factor);
	}
}

void Scene_Boss::update(float dt)
{
	
	Timer& _Timer = Timer::instance();

	_Timer.update();

	sound_mgr::instance().Frame(_Timer.delta());

	object_mgr::instance().update();

	collision_mgr::instance().update();

	Input_mgr::instance().update();
}

void Scene_Boss::initialize()
{
	// TOOD :: Scene Dependent Init 
	{



	}
}

void Scene_Boss::release()
{

}
