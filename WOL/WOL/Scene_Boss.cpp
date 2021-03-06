#include "pch.h"

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
#include "BOSS.h"
#include "game.h"
#include "Teleport.h"
#include "Tile_mgr.h"

Scene_Boss::~Scene_Boss() noexcept
{
	release();
};

void Scene_Boss::render(HDC hdc, std::pair<float, float> size_factor)
{
	Scene::render(hdc,size_factor);

	{
		auto FontOn = Font(hdc, L"", 20, RGB(255, 0, 0));

		object_mgr& obj_mgr = object_mgr::instance();
		vec cp = obj_mgr.camera_pos;

		Tile_mgr::instance().render(hdc, cp ,size_factor);
		obj_mgr.render(hdc, size_factor);
		Tile_mgr::instance().DecoRender(hdc, cp);
		obj_mgr.UIEffectRender(hdc, cp, size_factor);
		collision_mgr::instance().render(hdc, size_factor);
		Timer::instance().render(hdc);
	}
}

void Scene_Boss::update(float dt)
{
	Scene::update(dt);

	Timer& _Timer = Timer::instance();

	_Timer.update();

	sound_mgr::instance().Frame(dt);

	Tile_mgr::instance().update(dt);

	object_mgr::instance().update();

	Input_mgr::instance().update();

	collision_mgr::instance().update();
}

void Scene_Boss::initialize()
{
	Scene::initialize();

	Tile_mgr::instance().initialize();

	Tile_mgr::instance().Load_Tile(Tile_mgr::BossStageFileName);

	collision_mgr::instance().load_collision(collision_mgr::BossStageFileName);

	// TOOD :: Scene Dependent Init 
	{
		object_mgr& obj_mgr = object_mgr::instance();


		auto _Teleport = obj_mgr.insert_object<Teleport>();
		
		_Teleport->SetUp(PlayerSpawnLocation);

		game::_Player->_transform->_location = PlayerSpawnLocation;

		sound_mgr::instance().Stop("DUNGEON_BGM");
		SOUNDPLAY("BOSS_BGM", 1.f, true );

		auto _Boss = object_mgr::instance().insert_object<BOSS>();

		_Boss->SetUp(game::_Player, vec{ 1856,1570});
		
		manage_objs.push_back(_Teleport);
		manage_objs.push_back(_Boss);

	};
}

void Scene_Boss::release()
{

	Scene::release();

	Tile_mgr::instance().release();

	collision_mgr::instance().collision_tile_clear();

}
