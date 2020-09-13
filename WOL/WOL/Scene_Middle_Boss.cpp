#include "pch.h"
#include "Scene_Middle_Boss.h"

#include "Scene_Boss.h"
#include "collision_mgr.h"
#include "Scene_mgr.h"
#include "Input_mgr.h"
#include "timer.h"
#include "object_mgr.h"
#include "sound_mgr.h"
#include "Font.h"
#include "player.h"
#include "Camera.h"
#include "Tile_mgr.h"
#include "MIDDLE_BOSS.h"
#include "Teleport.h"

Scene_Middle_Boss::~Scene_Middle_Boss() noexcept
{
	release();
}

void Scene_Middle_Boss::render(HDC hdc, std::pair<float, float> size_factor)
{
	Scene::render(hdc, size_factor);

	{
		auto FontOn = Font(hdc, L"", 20, RGB(255, 0, 0));

		object_mgr& obj_mgr = object_mgr::instance();
		vec cp = obj_mgr.camera_pos;

		Tile_mgr::instance().render(hdc, cp, size_factor);
		obj_mgr.render(hdc, size_factor);
		Tile_mgr::instance().DecoRender(hdc, cp);
		obj_mgr.UIEffectRender(hdc, cp, size_factor);
		collision_mgr::instance().render(hdc, size_factor);
		Timer::instance().render(hdc);
	}
}

void Scene_Middle_Boss::update(float dt)
{
	Scene::update(dt);

	Timer& _Timer = Timer::instance();

	_Timer.update();

	sound_mgr::instance().Frame(DeltaTime);

	Tile_mgr::instance().update(dt);

	object_mgr::instance().update();

	Input_mgr::instance().update();

	collision_mgr::instance().update();
}

void Scene_Middle_Boss::initialize()
{
	Scene::initialize();

	Tile_mgr::instance().initialize();

	Tile_mgr::instance().Load_Tile(Tile_mgr::Middle_BossStageFileName);

	collision_mgr::instance().load_collision(collision_mgr::Middle_BossStageFileName);

	// TOOD :: Scene Dependent Init 
	{
		sound_mgr::instance().Stop("DUNGEON_BGM");
		SOUNDPLAY("BOSS_BGM", 1.f, true);

		object_mgr& obj_mgr = object_mgr::instance();

		auto _Player = obj_mgr.insert_object<Player>();
		auto _camera = obj_mgr.insert_object<Camera>();
		auto Midboss = object_mgr::instance().insert_object<MIDDLE_BOSS>();
		auto _Teleport = obj_mgr.insert_object<Teleport>();
	
		_Teleport->SetUp(PlayerSpawnLocation,true , ESceneID::EStage);
		Midboss->SetUp(_Player, vec{ 2700,1400 });
		_camera->_owner = _Player;
		obj_mgr._Camera = _camera;
		_Player->_transform->_location = PlayerSpawnLocation;
		
		manage_objs.push_back(_Teleport);
		manage_objs.push_back(Midboss);
		manage_objs.push_back(_Player);
		manage_objs.push_back(_camera);
	};
}

void Scene_Middle_Boss::release()
{
	Scene::release();

	Tile_mgr::instance().release();

	collision_mgr::instance().collision_tile_clear();
}
