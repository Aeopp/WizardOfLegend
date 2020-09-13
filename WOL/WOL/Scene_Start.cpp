#include "pch.h"
#include "Scene_Start.h"
#include "collision_mgr.h"
#include "Scene_mgr.h"
#include "Input_mgr.h"
#include "timer.h"
#include "object_mgr.h"
#include "sound_mgr.h"
#include "Bmp_mgr.h"
#include "Font.h"
#include "Mouse.h"
#include "UIStart.h"

void Scene_Start::render(HDC hdc, std::pair<float, float> size_factor)
{
	{
		auto FontOn = Font(hdc, L"", 20, RGB(255, 0, 0));
		object_mgr& obj_mgr = object_mgr::instance();

		obj_mgr.render(hdc, size_factor);
		obj_mgr.UIEffectRender(hdc, obj_mgr.camera_pos, size_factor);
	}
}

void Scene_Start::update(float dt)
{
	Scene::update(dt);

	Timer& _Timer = Timer::instance();

	_Timer.update();

	sound_mgr::instance().Frame(DeltaTime);

	object_mgr::instance().update();

	Input_mgr::instance().update();

	collision_mgr::instance().update();
}

void Scene_Start::initialize()
{
	AddFontResource(L"..\\..\\Resources\\Font\\NanumBarunGothicBold.ttf");

	Timer::instance().initialize();

	sound_mgr::instance().initialize();

	Bmp_mgr::instance().initialize();

	Input_mgr::instance().initialize();

	object_mgr::instance().initialize();

	// TOOD :: Scene Dependent Init 
	{
		sound_mgr::instance().Play("MAIN_MENU_BGM", true, 1.f);
		

		auto EMouse = object_mgr::instance().insert_object<Mouse>();

		auto _UIStart = object_mgr::instance().insert_object<UIStart>();
		if (!_UIStart)return;

		manage_objs.push_back(_UIStart);
	}
}

void Scene_Start::release()
{

	Scene::release();
}

Scene_Start::~Scene_Start() noexcept
{
	release();
}
