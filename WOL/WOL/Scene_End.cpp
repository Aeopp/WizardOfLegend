#include "pch.h"
#include "Scene_End.h"
#include "Scene_mgr.h"
#include "Input_mgr.h"
#include "timer.h"
#include "object_mgr.h"
#include "sound_mgr.h"
#include "Bmp_mgr.h"
#include "collision_mgr.h"
#include "Font.h"

Scene_End::~Scene_End() noexcept
{
	release();
}

void Scene_End::render(HDC hdc, std::pair<float, float> size_factor)
{
	{
		auto FontOn = Font(hdc, L"", 20, RGB(255, 0, 0));

		Timer::instance().render(hdc);
		object_mgr& obj_mgr = object_mgr::instance();
		obj_mgr.render(hdc, size_factor);
		obj_mgr.UIEffectRender(hdc, obj_mgr.camera_pos, size_factor);
		collision_mgr::instance().render(hdc, size_factor);
	}
};

void Scene_End::update(float dt)
{
	Timer& _Timer = Timer::instance();

	_Timer.update();

	sound_mgr::instance().Frame(_Timer.delta());

	object_mgr::instance().update();

	collision_mgr::instance().update();

	Input_mgr::instance().update();
}

void Scene_End::initialize()
{
	
}

void Scene_End::release()
{
	Bmp_mgr::instance().release();

	sound_mgr::instance().Release();

	object_mgr::instance().release();

	collision_mgr::instance().release();
}
