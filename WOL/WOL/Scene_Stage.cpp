#include "pch.h"
#include "Trigger.h"
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
#include "Prison.h"


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


		sound_mgr::instance().Stop("MAIN_MENU_BGM");
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
	


		TriggerSetUp(_Player);

		//auto archer = object_mgr::instance().insert_object<WizardBall>
		//	(_Player, vec{ 100,100 });

		//float Angle = 360.f / 16.f;
		//float s = 0.f;
		//float Distance = 400.f;
		//Timer::instance().event_regist_ReWhileDelta(100000000000.f, 10.f, [=]() {
		//	auto archer = object_mgr::instance().insert_object<ARCHER>(_Player, _Player->_transform->_location +
		//		math::RandVec() * math::Rand<int>({ -300,300 }));

		//	auto swordman = object_mgr::instance().insert_object<SwordMan>(_Player, _Player->_transform->_location +
		//		math::RandVec() * math::Rand<int>({ -300,300 }));

		//	auto wizard = object_mgr::instance().insert_object<WIZARD>(_Player, _Player->_transform->_location +
		//		math::RandVec() * math::Rand<int>({ -300,300 }));

		//	manage_objs.push_back(archer);
		//	manage_objs.push_back(swordman);

		//	manage_objs.push_back(wizard);
		//	});


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

void Scene_Stage::TriggerSetUp(std::weak_ptr<class Player> _Player)
{
	auto _Trigger = object_mgr::instance().insert_object<Trigger>();

	auto StartEvent = [_Trigger,_Player]() {

		std::vector< std::weak_ptr<class object>  > ReturnObjects;

		auto _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 1.f, Prison::EType::Hor,
			_Trigger, { 100,100 }, vec{ 1350,2450 });

		auto _Archer = object_mgr::instance().insert_object<ARCHER>(_Player,
			vec{ 1000,2000 });

		auto _Swordman= object_mgr::instance().insert_object<SwordMan>(_Player,
			vec{ 1000,2500 });

		auto _Wizard = object_mgr::instance().insert_object<WIZARD>(_Player,
			vec{ 1000,3000 });
		
		ReturnObjects.push_back(_Archer);
		ReturnObjects.push_back(_Wizard);
		ReturnObjects.push_back(_Swordman);
		return ReturnObjects;
	};

	auto Event_1 = [_Player]()
	{
		std::vector< std::weak_ptr<class object>  > ReturnObjects;

		auto _Archer = object_mgr::instance().insert_object<ARCHER>(_Player,
			vec{ 1000,2000 });

		auto _Swordman = object_mgr::instance().insert_object<SwordMan>(_Player,
			vec{ 1000,2500 });

		auto _Wizard = object_mgr::instance().insert_object<WIZARD>(_Player,
			vec{ 1000,3000 });

		ReturnObjects.push_back(_Archer);
		ReturnObjects.push_back(_Wizard);
		ReturnObjects.push_back(_Swordman);
		return ReturnObjects;
	};

	auto Event_2 = [_Player]()
	{
		std::vector< std::weak_ptr<class object>  > ReturnObjects;

		auto _Archer = object_mgr::instance().insert_object<ARCHER>(_Player,
			vec{ 1000,2000 });

		auto _Swordman = object_mgr::instance().insert_object<SwordMan>(_Player,
			vec{ 1000,2500 });

		auto _Wizard = object_mgr::instance().insert_object<WIZARD>(_Player,
			vec{ 1000,3000 });

		ReturnObjects.push_back(_Archer);
		ReturnObjects.push_back(_Wizard);
		ReturnObjects.push_back(_Swordman);
		return ReturnObjects;
	};

	std::queue<std::function<std::vector<std::weak_ptr<object>>()>> EventQ;
	EventQ.push(std::move(Event_1));
	EventQ.push(std::move(Event_2 ));

	_Trigger->SetUp({ 200,200 }, vec{ 1350,2450}, std::move(StartEvent),
		[]() {}, std::move(EventQ  ) );


	manage_objs.push_back(_Trigger);
}
