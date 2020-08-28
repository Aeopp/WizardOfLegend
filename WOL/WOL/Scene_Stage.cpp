#include "pch.h"
#include "Scene_Stage.h"
#include "collision_mgr.h"
#include "Scene_mgr.h"
#include "Input_mgr.h"
#include "timer.h"
#include "object_mgr.h"
#include "sound_mgr.h"
#include "Bmp_mgr.h"
#include "Font.h"
#include "Camera.h"
#include "player.h"
#include "SwordMan.h"
#include "ARCHER.h"
#include "WIZARD.h"
#include "BOSS.h"
#include "MIDDLE_BOSS.h"
#include "Tile_mgr.h"


void Scene_Stage::render(HDC hdc, std::pair<float, float> size_factor)
{
	Scene::render(hdc,size_factor);

	{
		auto FontOn = Font(hdc, L"", 20, RGB(255, 0, 0));

		Tile_mgr::instance().render(hdc, size_factor);
		object_mgr::instance().render(hdc, size_factor);
		collision_mgr::instance().render(hdc, size_factor);
		Timer::instance().render(hdc);
	}
}

void Scene_Stage::update(float dt)
{
	Scene::update(dt);

	Timer& _Timer = Timer::instance();

	_Timer.update();

	sound_mgr::instance().Frame(_Timer.delta());

	Tile_mgr::instance().update(dt);

	object_mgr::instance().update();

	collision_mgr::instance().update();

	Input_mgr::instance().update();
}

void Scene_Stage::initialize()
{
	Scene::initialize();

	Tile_mgr::instance().initialize();

	Tile_mgr::instance().Load_Tile();

	collision_mgr::instance().load_collision(collision_mgr::StageFileName);

	// TOOD :: Scene Dependent Init 
	{
		object_mgr& obj_mgr = object_mgr::instance();

		auto _ptr = obj_mgr.insert_object<Player>();

		auto _camera = obj_mgr.insert_object<Camera>();
		_camera->_owner = _ptr;

		obj_mgr._Camera = _camera;

		manage_objs.push_back(_camera);

		for (int i = 1; i < 10; ++i)
		{
			int x = math::Rand_N<int>({100, 1400});
			int y = math::Rand_N<int>({100,900});
			
			auto mob = obj_mgr.insert_object<SwordMan>(_ptr);
			mob->_transform->_location = vec{ x,y};

			manage_objs.push_back(mob);
		}

		for (int i = 1; i < 10; ++i)
		{
			int x = math::Rand_N<int>({ 100, 1400 });
			int y = math::Rand_N<int>({ 100,900 });

			auto mob = obj_mgr.insert_object<ARCHER>(_ptr);
			mob->_transform->_location = vec{ x,y };

			manage_objs.push_back(mob);
		}

		for (int i = 1; i < 10; ++i)
		{
			int x = math::Rand_N<int>({ 100, 1400 });
			int y = math::Rand_N<int>({ 100,900 });

			auto mob = obj_mgr.insert_object<BOSS>(_ptr);
			mob->_transform->_location = vec{ x,y };

			manage_objs.push_back(mob);
		}
		for (int i = 1; i < 10; ++i)
		{
			int x = math::Rand_N<int>({ 100, 1400 });
			int y = math::Rand_N<int>({ 100,900 });

			auto mob = obj_mgr.insert_object<MIDDLE_BOSS>(_ptr);
			mob->_transform->_location = vec{ x,y };

			manage_objs.push_back(mob);
		}
		for (int i = 1; i < 10; ++i)
		{
			int x = math::Rand_N<int>({ 100, 1400 });
			int y = math::Rand_N<int>({ 100,900 });

			auto mob = obj_mgr.insert_object<WIZARD>(_ptr);
			mob->_transform->_location = vec{ x,y };

			manage_objs.push_back(mob);
		}
	}
}

void Scene_Stage::release()
{
	Scene::release();

	Tile_mgr::instance().release();

	collision_mgr::instance().collision_tile_clear();
}

Scene_Stage::~Scene_Stage() noexcept
{
	release();
}
