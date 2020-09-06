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
#include "Debuger.h"
#include "ARCHER.h"
#include "sound_mgr.h"
#include "WizardBall.h"


void Scene_Stage::render(HDC hdc, std::pair<float, float> size_factor)
{
	Scene::render(hdc,size_factor);

	{
		auto FontOn = Font(hdc, L"", 20, RGB(255, 0, 0));

		object_mgr& obj_mgr = object_mgr::instance();
		vec cp = obj_mgr.camera_pos;

		Tile_mgr::instance().render(hdc, cp,size_factor);
		obj_mgr.render(hdc, size_factor);
		Tile_mgr::instance().DecoRender(hdc, cp);
		obj_mgr.UIEffectRender(hdc, cp, size_factor);
		collision_mgr::instance().render(hdc, size_factor);
		Timer::instance().render(hdc);
	}
}

void Scene_Stage::update(float dt)
{
	Input_mgr& _Input = Input_mgr::instance();

	Scene::update(dt);

	Timer& _Timer = Timer::instance();

	_Timer.update();

	sound_mgr::instance().Frame(_Timer.delta());

	Tile_mgr::instance().update(dt);

	object_mgr::instance().update();

	if (_Input.Key_Down('0'))
	{
		Scene_mgr::instance().Scene_Change(ESceneID::EBoss);
	}

	_Input.update();

	collision_mgr::instance().update();
}

void Scene_Stage::initialize()
{
	Scene::initialize();

	Tile_mgr& _Tile_mgr = Tile_mgr::instance();

	_Tile_mgr.initialize();

	_Tile_mgr.Load_Tile(Tile_mgr::StageFileName);

	collision_mgr::instance().load_collision(collision_mgr::StageFileName);

	auto Effect_SUMMON = object_mgr::instance().insert_object<Effect>
		(PlayerSpawnLocation.x,+100,
			L"SUMMON", layer_type::EEffect, 8, 0, 1.0f, 1.0f, 225, 730,
			1.f, 1.6f);

		SOUNDPLAY("DUNGEON_BGM", 1.f, true);
		SOUNDPLAY("teleport", 1.f, false);

	/*late_initialize(int ImgLocationX, int ImgLocationY,
		std::wstring ImgKey, layer_type layer_ID, int AnimColNum,
		int AnimRowIndex, float Duration, float AnimDuration,
		int PaintSizeX, int PaintSizeY, float ScaleX, float ScaleY);*/
//	 TOOD :: Scene Dependent Init 
	{
		object_mgr& obj_mgr = object_mgr::instance();

		auto _Player = obj_mgr.insert_object<Player>();
		_Player->_transform->_location = PlayerSpawnLocation;

		auto _camera = obj_mgr.insert_object<Camera>();
		_camera->_owner = _Player;

		obj_mgr._Camera = _camera;
	
		float Angle = 360.f / 16.f;
		float s = 0.f;
		float Distance = 400.f;

		for (int i = 0; i < 1; ++i)
		{
			/*auto SWORDMAN = object_mgr::instance().insert_object
			<ARCHER>(_Player,_Player->_transform->_location + vec{ 100,0 });
*/
			auto Midboss = object_mgr::instance().insert_object<MIDDLE_BOSS>();

			Midboss->SetUp(_Player, PlayerSpawnLocation + vec{ 0,-100 });

			manage_objs.push_back(Midboss);
			//auto  ar= object_mgr::instance().insert_object<ARCHER>(_Player, _Player->_transform->_location + vec{ 100,0 });

		/*
			auto Midboss = object_mgr::instance().insert_object<MIDDLE_BOSS>();
			Midboss->SetUp(_Player, PlayerSpawnLocation + math::RandVec() * math::Rand<int>({ 500,1000 }));

			manage_objs.push_back(Midboss);*/
		/*	s += Angle;
			vec v = PlayerSpawnLocation;
			vec w{ Distance * std::cosf(s),
			Distance * std::sinf(s) };
			v += w;


			vec InitPos = v +
				math::RandVec() * math::Rand<float>({ -100,100 });
			Monster::CardEffect(InitPos, WizardBall::SummonCardImgKey);
			auto sp_WizBall = WizardBall::BallCast();
			sp_WizBall->_transform->_location = InitPos;
			sp_WizBall->wp_AttackTarget = _Player;

			manage_objs.push_back(sp_WizBall);

			auto qwe = object_mgr::instance().insert_object<ARCHER>(_Player, v);
			
			manage_objs.push_back(qwe);*/
		}
		
		manage_objs.push_back(_camera);
		manage_objs.push_back(_Player);
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
