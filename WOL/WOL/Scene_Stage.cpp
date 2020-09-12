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
#include "ArcanaCard.h"
#include "NPC.h"
#include "UIItem.h"
#include "player_info.h"
#include "UIInventory.h"
#include "Teleport.h"






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
		(PlayerSpawnLocation.x, PlayerSpawnLocation.y -400,
			L"SUMMON", layer_type::EEffect, 8, 0, 1.0f, 1.0f, 225, 730,
			1.f, 1.6f);
	
		sound_mgr::instance().Stop("MAIN_MENU_BGM");
		SOUNDPLAY("DUNGEON_BGM", 1.f, true);

	{
			object_mgr& obj_mgr = object_mgr::instance();

			auto _Player = obj_mgr.insert_object<Player>();
			_Player->_transform->_location = PlayerSpawnLocation;

			auto _camera = obj_mgr.insert_object<Camera>();
			_camera->_owner = _Player;
			obj_mgr._Camera = _camera;

			auto _Teleport = obj_mgr.insert_object<Teleport>();
			_Teleport->SetUp(PlayerSpawnLocation, false);
		
		store_set_up(_Player);
		TriggerSetUp(_Player);
		manage_objs.push_back(_Teleport);
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

void Scene_Stage::player_set_up()
{
	
}

void Scene_Stage::store_set_up(std::weak_ptr<class Player> _Player)
{
	auto _Npc = object_mgr::instance().insert_object<NPC>();
	_Npc->SetUp(vec{ 2800,150 });

	std::weak_ptr wp_Player = _Player;

	auto _Potion = object_mgr::instance().insert_object<UIItem>();
	_Potion->SetUp({ 2650,340 }, L"POTION", { 106,155 },
		[wp_Player]() {
		auto sp_Player = wp_Player.lock();
		if (!sp_Player) return false;
		if (!sp_Player->_player_info)return false;
		if (sp_Player->_player_info->gold < 100)return false;

		sp_Player->_player_info->AddHp(sp_Player->_player_info->max_hp);
		sp_Player->_player_info->AddGold(-100);

		std::wstring str = L"-";
		str += std::to_wstring(100);

		vec randvec = math::RandVec();
		randvec.y = (abs(randvec.y));
		object_mgr::instance().TextEffectMap[RGB(223, 207, 0)].
			push_back({ sp_Player->_transform->_location,randvec,
			   1.f,100,str });
		return true;
	});

	auto _ArmorItem = object_mgr::instance().insert_object<UIItem>();
	_ArmorItem->SetUp({ 2850,340 }, L"GAIA_ARMOR_CARD", { 106,172 },
		[wp_Player]() {
		auto sp_Player = wp_Player.lock();
		if (!sp_Player) return false;
		if (!sp_Player->_player_info)return false;
		if (sp_Player->_player_info->gold < 150)return false;
		 
		sp_Player->_player_info->AddGold(-150);

		std::wstring str = L"-";
		str += std::to_wstring(150);

		vec randvec = math::RandVec();
		randvec.y = (abs(randvec.y));
		object_mgr::instance().TextEffectMap[RGB(223, 207, 0)].
			push_back({ sp_Player->_transform->_location,randvec,
			   1.f,150,str });

		auto iter = std::find_if(std::begin(UIInventory::SlotInfoMap),
			std::end(UIInventory::SlotInfoMap),
			[](auto& _KeySlot) {
			return _KeySlot.second._Skill == ESkill::ARMOR;
		});

		if (iter != std::end(UIInventory::SlotInfoMap))
		{
			iter->second.bAcquire = true;
			return true;
		}

		return false;
	});
	
	manage_objs.push_back(_Npc);
	manage_objs.push_back(_ArmorItem);
	manage_objs.push_back(_Potion);
}

Scene_Stage::~Scene_Stage() noexcept
{
	release();
}

std::shared_ptr<class object> Scene_Stage::Stage_1(std::weak_ptr<class Player> _Player)
{
	auto _Trigger = object_mgr::instance().insert_object<Trigger>();

	std::weak_ptr<class object> wp_Trigger = _Trigger;

	// Start 
	auto StartEvent = [wp_Trigger, _Player]() {

		std::vector< std::weak_ptr<class object>  > ReturnObjects;

		vec PrisonLocation = vec{ 1660,2385 };
		auto _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 1.5, Prison::EType::Ver,
			wp_Trigger, { 100,250 }, PrisonLocation);

		vec SwordManLocation = vec{ 600, 2440 };

		ReturnObjects += Monster::TypeMatchMonstersSpawn<SwordMan>(_Player,
			{ SwordManLocation });

		return ReturnObjects;
	};
	// 1 
	auto Event_1 = [_Player]()
	{
		std::vector< std::weak_ptr<class object>> ReturnObjects;

		std::vector<vec> MonsterLocation{ {561,2344} , { 505,2718} };

		ReturnObjects += Monster::TypeMatchMonstersSpawn<SwordMan>(_Player, MonsterLocation);

		return ReturnObjects;
	};
	// 2 
	auto Event_2 = [_Player]()
	{
		std::vector< std::weak_ptr<class object>  > ReturnObjects;

		vec FireCardLocation = { 900, 2000 };

		auto fire_card = object_mgr::instance().
			insert_object<ArcanaCard>(FireCardLocation, ESkill::FIRE, L"FIRE_DRAGON_CARD");

		std::vector<vec> MonsterLocation{ {1361,2545} , { 474,2484} };

		ReturnObjects += Monster::TypeMatchMonstersSpawn<ARCHER>(_Player, MonsterLocation);

		return ReturnObjects;
	};
	// 3
	auto Event_3 = [_Player]()
	{
		std::vector< std::weak_ptr<class object>  > ReturnObjects;

		std::vector<vec> MonsterLocation{ {900,2200} , { 900,3100} };

		ReturnObjects += Monster::TypeMatchMonstersSpawn<WIZARD>(_Player, MonsterLocation);

		return ReturnObjects;
	};
	
	auto EndEvent = [BlastCardLocation =this->PlayerSpawnLocation]()
	{
		auto blast_card = object_mgr::instance().
		insert_object<ArcanaCard>(BlastCardLocation, ESkill::BLAST, L"ICE_BLAST_CARD");
	};

	std::queue<std::function<std::vector<std::weak_ptr<object>>()>> EventQ;
	EventQ.push(std::move(Event_1));
	EventQ.push(std::move(Event_2));
	EventQ.push(std::move(Event_3));
	vec  EventZoneSize = { 500,500 };
	vec TriggerLocation = { 1350,2450 };

	_Trigger->SetUp({ 200,200 }, vec{ 1350,2450 },
		std::move(StartEvent),
		std::move( EndEvent ) , std::move(EventQ));

	return _Trigger;
}

std::shared_ptr<object> Scene_Stage::Stage_2(std::weak_ptr<Player> _Player)
{
	auto _Trigger = object_mgr::instance().insert_object<Trigger>();

	std::weak_ptr<class object> wp_Trigger = _Trigger;

	// Start 
	auto StartEvent = [wp_Trigger, _Player]() {

		std::vector< std::weak_ptr<class object>> ReturnObjects;

		vec PrisonLocation = vec{ 3779,2381};
		auto _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 0.8, Prison::EType::Ver,
			wp_Trigger, { 100,200 }, PrisonLocation);
		
		 PrisonLocation = vec{ 4750,2400};
		 _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 0.8, Prison::EType::Ver,
			wp_Trigger, { 150,200 }, PrisonLocation);
		
		 PrisonLocation = vec{ 4030,2920};
		 _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.1, 0.7, Prison::EType::Hor,
			wp_Trigger, { 150,250 }, PrisonLocation);
		
		 PrisonLocation = vec{ 4160,2040 };
		 _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(0.9, 0.7, Prison::EType::Hor,
			wp_Trigger, { 150,250 }, PrisonLocation);

		std::vector<vec> SwordManLocations = { { 3983, 2282 },{4514,2533} };
		std::vector<vec> ArcherLocations = { { 4035, 2314},{4441,2502} };

		ReturnObjects += Monster::TypeMatchMonstersSpawn<SwordMan>(_Player,
			 SwordManLocations );
		
		ReturnObjects += Monster::TypeMatchMonstersSpawn<ARCHER>(_Player,
			ArcherLocations);
		
		return ReturnObjects;
	};

	auto Event_1 = [_Player]()
	{
		std::vector< std::weak_ptr<class object>> ReturnObjects;

		std::vector<vec> MonsterLocation{ {4007,2275} , { 4530,2272},{3970,2550}
		,{4500,2500}};

		ReturnObjects += Monster::TypeMatchMonstersSpawn<SwordMan>(_Player, MonsterLocation);

		return ReturnObjects;
	};

	auto EndEvent = []()
	{
		vec CrystalCardLocation = vec{ 4776,4292};

		auto crystal_card = object_mgr::instance().
			insert_object<ArcanaCard>(CrystalCardLocation, ESkill::CRYSTAL, L"ICE_KRYSTAL_CARD");
	};
	
	std::queue<std::function<std::vector<std::weak_ptr<object>>()>> EventQ;
	EventQ.push(std::move(Event_1));
	std::pair<int,int>  EventZoneSize = { 200,200};
	vec TriggerLocation = { 4100,2409};
	std::pair<vec, vec> CameraRange = { { 4200,2300} ,  { 4400,2500} };
	_Trigger->SetUp(EventZoneSize, TriggerLocation,
		std::move(StartEvent),
		std::move(EndEvent) , std::move(EventQ),true,
		CameraRange);

	return _Trigger;
}

std::shared_ptr<object> Scene_Stage::Stage_3(std::weak_ptr<Player> _Player)
{
	auto _Trigger = object_mgr::instance().insert_object<Trigger>();

	std::weak_ptr<class object> wp_Trigger = _Trigger;

	// Start 
	auto StartEvent = [wp_Trigger, _Player]() {

		std::vector< std::weak_ptr<class object>> ReturnObjects;

		vec PrisonLocation = vec{ 4260,4232};
		auto _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 1, Prison::EType::Ver,
			wp_Trigger, { 100,150 }, PrisonLocation);
		

		std::vector<vec> WizardBallLocations = 
		{
			{ 4817, 3971 },{4498 ,  4294} , {5113  , 4341 } , {4808 ,4785 }
		};
		
		for(auto& Location : WizardBallLocations)
		{
			auto sp_Ball=
			object_mgr::instance().insert_object<WizardBall>(_Player, std::move(Location));
			
			ReturnObjects.push_back(sp_Ball);
		}
		
		return ReturnObjects;
	};

	auto Event_1 = [_Player]()
	{
		std::vector< std::weak_ptr<class object>> ReturnObjects;
		
		std::vector<vec> WizardBallLocations =
		{
			{ 4817, 3971 },{4498 ,  4294} , {5113  , 4341 } , {4808 ,4785 },
			{ 4900, 3971 },{4598 ,  4294} , {5213  , 4341 } , {4908 ,4785 }
		};
		
		for (auto& Location : WizardBallLocations)
		{
			auto sp_Ball =
				object_mgr::instance().insert_object<WizardBall>(_Player, std::move(Location));

			ReturnObjects.push_back(sp_Ball);
		}
		
		return ReturnObjects;
	};

	std::queue<std::function<std::vector<std::weak_ptr<object>>()>> EventQ;
	EventQ.push(std::move(Event_1));
	std::pair<int, int>  EventZoneSize = { 200,200 };
	vec TriggerLocation = { 4861,4326 };
	std::pair<vec, vec> CameraRange = { { 4700,4200} ,  { 4900,4400} };
	_Trigger->SetUp(EventZoneSize, TriggerLocation,
		std::move(StartEvent),
		[] {}, std::move(EventQ), true,
		CameraRange);

	return _Trigger;
}

std::shared_ptr<object> Scene_Stage::Stage_4(std::weak_ptr<Player> _Player)
{
	return {};
}

std::shared_ptr<object> Scene_Stage::Stage_5(std::weak_ptr<Player> _Player)
{
	return {};
}

std::shared_ptr<object> Scene_Stage::Stage_6(std::weak_ptr<Player> _Player)
{
	return {};
}

std::shared_ptr<object> Scene_Stage::Stage_7(std::weak_ptr<Player> _Player)
{
	return {}; 
}


void Scene_Stage::TriggerSetUp(std::weak_ptr<class Player> _Player)
{
	manage_objs.push_back(Stage_1(_Player));
	manage_objs.push_back(Stage_2(_Player));
	manage_objs.push_back(Stage_3(_Player));
	manage_objs.push_back(Stage_4(_Player));
	manage_objs.push_back(Stage_5(_Player));
	manage_objs.push_back(Stage_6(_Player));
	manage_objs.push_back(Stage_7(_Player));
}
