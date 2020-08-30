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
#include "GoldEffect.h"


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

	Tile_mgr& _Tile_mgr = Tile_mgr::instance();

	_Tile_mgr.initialize();

	_Tile_mgr.Load_Tile(Tile_mgr::StageFileName);

	collision_mgr::instance().load_collision(collision_mgr::StageFileName);

	auto Effect_SUMMON = object_mgr::instance().insert_object<Effect>
		(PlayerSpawnLocation.x,0,
			L"SUMMON", layer_type::EEffect, 8, 0, 0.9f, 0.9f, 225, 730,
			1.f, 1.6f);

	/*late_initialize(int ImgLocationX, int ImgLocationY,
		std::wstring ImgKey, layer_type layer_ID, int AnimColNum,
		int AnimRowIndex, float Duration, float AnimDuration,
		int PaintSizeX, int PaintSizeY, float ScaleX, float ScaleY);*/
	// TOOD :: Scene Dependent Init 
	{
		object_mgr& obj_mgr = object_mgr::instance();


		auto _Player = obj_mgr.insert_object<Player>();
		_Player->_transform->_location = PlayerSpawnLocation;

		auto _camera = obj_mgr.insert_object<Camera>();
		_camera->_owner = _Player;

		obj_mgr._Camera = _camera;


		for (int i = 6; i < 20; ++i)
		{
			auto _gold = obj_mgr.insert_object<GoldEffect>(i * 100, i * 100,
				L"MONEY", layer_type::EEffect, 2,
				math::Rand<int>({ 1,3 }), FLT_MAX, 0.2f, 24, 24, 1.f, 1.f);

			_gold->_transform->_location = { i * 100, i * 100};
			_gold->_owner = _Player;

		}


		manage_objs.push_back(_camera);
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
