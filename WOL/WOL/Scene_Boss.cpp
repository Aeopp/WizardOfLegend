#include "pch.h"

#include "BOSS.h"
#include "MIDDLE_BOSS.h"

#include "ARCHER.h"
#include "SwordMan.h"
#include "WIZARD.h"
#include "Scene_Boss.h"
#include "collision_mgr.h"
#include "Scene_mgr.h"
#include "Input_mgr.h"
#include "timer.h"
#include "object_mgr.h"
#include "sound_mgr.h"
#include "Bmp_mgr.h"
#include "Font.h"
#include "player.h"
#include "Camera.h"
#include "Tile_mgr.h"
#include "Monster.h"
#include "ArcanaCard.h"

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

	sound_mgr::instance().Frame(_Timer.delta());

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

		auto _Player = obj_mgr.insert_object<Player>();

		auto _camera = obj_mgr.insert_object<Camera>();
		_camera->_owner = _Player;
		obj_mgr._Camera = _camera;

		manage_objs.push_back(_camera);
		manage_objs.push_back(_Player);

		_Player->_transform->_location = PlayerSpawnLocation;

		float Angle = 360.f / 16.f;
		float s = 0.f;
		float Distance = 400.f;

		sound_mgr::instance().Stop("DUNGEON_BGM");
		SOUNDPLAY("BOSS_BGM", 1.f, true );

			auto Midboss = object_mgr::instance().insert_object<MIDDLE_BOSS>();

			Midboss->SetUp(_Player, vec{ 2700,1400 });

			manage_objs.push_back(Midboss);
		

			obj_mgr.insert_object<ArcanaCard>(PlayerSpawnLocation
				+ vec{ 200,0 }, ESkill::BLAST, L"ICE_BLAST_CARD");

			obj_mgr.insert_object<ArcanaCard>(PlayerSpawnLocation
				+ vec{ -200,0 }, ESkill::FIRE, L"FIRE_DRAGON_CARD");

			obj_mgr.insert_object<ArcanaCard>(PlayerSpawnLocation
				+ vec{ 0 ,-200 }, ESkill::CRYSTAL, L"ICE_KRYSTAL_CARD");

			obj_mgr.insert_object<ArcanaCard>(PlayerSpawnLocation
				+ vec{ 0 ,+200 }, ESkill::ARMOR, L"GAIR_ARMOR_CARD");

			obj_mgr.insert_object<ArcanaCard>(PlayerSpawnLocation
				+ vec{ 0 ,-400 }, ESkill::ARMOR, L"BOOMERANG_CARD");
	/*	for (int i = 0; i < 100; ++i)
		{
			Timer::instance().event_regist(time_event::EOnce, 10*i, [=,this]() {
				float s = 0.f;	s += Angle;
				vec v = PlayerSpawnLocation;
				vec w{ Distance * std::cosf(s),
				Distance * std::sinf(s) };
				v += w;
				v += math::RandVec() * math::Rand<int>({ -200, 200 });
				Monster::CardEffect(v, WIZARD::SummonCardImgKey);

				auto archer = object_mgr::instance().insert_object<WIZARD>
					(_Player, v);
				manage_objs.push_back(archer);
				return true;
				});
		}
		for (int i = 0; i < 100; ++i)
		{
			Timer::instance().event_regist(time_event::EOnce, 10 * i, [=, this]() {
				float s = 0.f;
				s += Angle;
				vec v = PlayerSpawnLocation;
				vec w{ Distance * std::cosf(s),
				Distance * std::sinf(s) };
				v += w;
				v += math::RandVec() * math::Rand<int>({ -200, 200 });
				Monster::CardEffect(v, SwordMan::SummonCardImgKey);

				auto archer = object_mgr::instance().insert_object<SwordMan>
					(_Player, v);
				manage_objs.push_back(archer);
				return true;
				});
		}
		for (int i = 0; i < 100; ++i)
		{
			Timer::instance().event_regist(time_event::EOnce, 10 * i, [=, this]() {
				float s = 0.f;	s += Angle;
				vec v = PlayerSpawnLocation;
				vec w{ Distance * std::cosf(s),
				Distance * std::sinf(s) };
				v += w;
				v += math::RandVec() * math::Rand<int>({ -200, 200 });
				Monster::CardEffect(v, ARCHER::SummonCardImgKey);

				auto archer = object_mgr::instance().insert_object<ARCHER>
					(_Player, v);
				manage_objs.push_back(archer);
				return true;
				});
		}*/
		manage_objs.push_back(_Player);
		manage_objs.push_back(_camera);
	};
}

void Scene_Boss::release()
{

	Scene::release();

	Tile_mgr::instance().release();

	collision_mgr::instance().collision_tile_clear();

}
