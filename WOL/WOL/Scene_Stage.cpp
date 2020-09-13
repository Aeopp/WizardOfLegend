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
#include "game.h"
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

	sound_mgr::instance().Frame(DeltaTime);

	Tile_mgr::instance().update(dt);

	object_mgr::instance().update();


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
	
		sound_mgr::instance().Stop("MAIN_MENU_BGM");
		sound_mgr::instance().Stop("BOSS_BGM");
		SOUNDPLAY("DUNGEON_BGM", 1.f, true);

	{
			object_mgr& obj_mgr = object_mgr::instance();

			game::instance().PlayerInit();
		
			game::_Player->_transform->_location = PlayerLocation;
			auto _camera = obj_mgr.insert_object<Camera>();
			_camera->_owner = game::_Player;
			obj_mgr._Camera = _camera;

			auto _Teleport = obj_mgr.insert_object<Teleport>();
			_Teleport->SetUp(PlayerLocation);
		
		store_set_up(game::_Player);
		TriggerSetUp(game::_Player);
		manage_objs.push_back(_Teleport);
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

		auto iter = std::find_if(std::begin(game::SlotInfoMap),
			std::end(game::SlotInfoMap),
			[](auto& _KeySlot) {
			return _KeySlot.second._Skill == ESkill::ARMOR;
		});

		if (iter != std::end(game::SlotInfoMap))
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
			wp_Trigger, { 200,250 }, PrisonLocation);
		_Prison->SetCollisionCorrection(vec{ 100,100});
		
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
	
	auto EndEvent = [BlastCardLocation =this->PlayerLocation]()
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
			wp_Trigger, { 150,180 }, PrisonLocation);
		_Prison->SetCollisionCorrection(vec{ -120,0 });
		
		 PrisonLocation = vec{ 4750,2400};
		 _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 0.8, Prison::EType::Ver,
			wp_Trigger, { 150,150 }, PrisonLocation);
		_Prison->SetCollisionCorrection(vec{ 120,0 });
		
		 PrisonLocation = vec{ 4030,2920};
		 _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.1, 0.7, Prison::EType::Hor,
			wp_Trigger, { 170,200}, PrisonLocation);
		_Prison->SetCollisionCorrection(vec{ 0,+100 });
		
		 PrisonLocation = vec{ 4160,2040 };
		 _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(0.9, 0.7, Prison::EType::Hor,
			wp_Trigger, { 170,200}, PrisonLocation);
		_Prison->SetCollisionCorrection(vec{ 0,-100 });

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
	std::pair<vec, vec> CameraRange = { { 4100,2200} ,  { 4500,2600} };
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

		std::vector<std::weak_ptr<class object>> ReturnObjects;

		vec PrisonLocation = vec{ 4260,4232};
		auto _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 1, Prison::EType::Ver,
			wp_Trigger, { 150,250}, PrisonLocation);
		_Prison->SetCollisionCorrection(vec{ -100,0 });

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
	std::pair<vec, vec> CameraRange = { { 4600,3900} ,  { 5000,4700} };
	_Trigger->SetUp(EventZoneSize, TriggerLocation,
		std::move(StartEvent),
		[] {}, std::move(EventQ), true,
		CameraRange);

	return _Trigger;
}

std::shared_ptr<object> Scene_Stage::Stage_4(std::weak_ptr<Player> _Player)
{
	auto _Trigger = object_mgr::instance().insert_object<Trigger>();

	std::weak_ptr<class object> wp_Trigger = _Trigger;

	// Start 
	auto StartEvent = [wp_Trigger, _Player]() {

		std::vector< std::weak_ptr<class object>> ReturnObjects;

		vec PrisonLocation = vec{ 2849,3852};
		auto _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 1, Prison::EType::Ver,
			wp_Trigger, { 200,220 }, PrisonLocation);
		_Prison->SetCollisionCorrection(vec{ -100, 0 } );

		PrisonLocation = vec{ 3340,4587};
		_Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(0.7, 1, Prison::EType::Hor,
			wp_Trigger, { 120,250 }, PrisonLocation);
		_Prison->SetCollisionCorrection(vec{ 0, 70});
		
		vec SpawnInitCenter = vec{ 3346,3980 };
		int32_t MonsterNum = 12;
		float Angle = 360.f / (float)MonsterNum;
		vec InitDir = math::dir_from_angle(Angle);
		float Radius = 300.f;
		std::vector<vec> Locations;
		Locations.reserve(MonsterNum);

		static auto SpawnGenerater = [SpawnInitCenter, InitDir, Radius, Angle]()mutable
		{
			vec SpawnLocation = SpawnInitCenter + InitDir * Radius;
			InitDir = math::rotation_dir_to_add_angle(InitDir, Angle);
			return SpawnLocation;
		};
		
		std::generate_n(std::back_inserter(Locations), MonsterNum,SpawnGenerater);

		ReturnObjects+=Monster::TypeMatchMonstersSpawn<ARCHER>(_Player, std::move( Locations ) );

		return ReturnObjects;
	};

	std::queue<std::function<std::vector<std::weak_ptr<object>>()>> EventQ;
	std::pair<int, int>  EventZoneSize = { 200,200 };
	vec TriggerLocation = { 3337,3957};
	std::pair<vec, vec> CameraRange = { { 3200,3700} ,  { 3500,4300} };
	_Trigger->SetUp(EventZoneSize, TriggerLocation,
		std::move(StartEvent),
		[] {}, std::move(EventQ), true,
		CameraRange);

	return _Trigger;
}

std::shared_ptr<object> Scene_Stage::Stage_5(std::weak_ptr<Player> _Player)
{
	auto _Trigger = object_mgr::instance().insert_object<Trigger>();

	std::weak_ptr<class object> wp_Trigger = _Trigger;

	// Start 
	auto StartEvent = [wp_Trigger, _Player]() {

		std::vector< std::weak_ptr<class object>> ReturnObjects;

		vec PrisonLocation = vec{ 1912,4658};
		auto _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 1, Prison::EType::Ver,
			wp_Trigger, { 100,150 }, PrisonLocation);

		std::vector<vec> Locations=
		{
			{362,4029},{1524,4035},{379,4896},{1547,4911}
		};

		ReturnObjects+=Monster::TypeMatchMonstersSpawn<WIZARD>(_Player, std::move(Locations));

		Locations = {{582,4216},{1118,4272},{860,4613}};
		
		ReturnObjects += Monster::TypeMatchMonstersSpawn<SwordMan>(_Player, std::move(Locations));

		return ReturnObjects;
	};

	auto Event_1 = [_Player]()
	{
		std::vector< std::weak_ptr<class object>> ReturnObjects;

		float Distance = 150.f;
		std::vector<vec> Centers = {{347,4029} , {1469,4018},
		{395,4839} , {1534,4880} , {854,4444}  };
		std::vector<vec> Locations;
		Locations.reserve(Centers.size() * 3);
		auto Location_Push_iter = std::back_inserter(Locations);
		
		for(const auto& Center : Centers)
		{
			Location_Push_iter  = Center + ( math::dir_from_angle(45)* Distance );
			Location_Push_iter  = Center + (math::dir_from_angle(135) * Distance);
			Location_Push_iter  = Center + (math::dir_from_angle(270) * Distance);
		}

		ReturnObjects += Monster::TypeMatchMonstersSpawn<SwordMan>(_Player, std::move(Locations));

		return ReturnObjects;
	};

	auto EndEvent = []
	{
		vec TelePort_Location = { 856,4464 };
		
		auto _Teleport = object_mgr::instance().insert_object<Teleport>();
		_Teleport->SetUp(TelePort_Location,true ,ESceneID::EMIDDLE_BOSS);
	};

	std::queue<std::function<std::vector<std::weak_ptr<object>>()>> EventQ;
	EventQ.push(std::move(Event_1));
	std::pair<int, int>  EventZoneSize = { 200,200 };
	vec TriggerLocation = { 856,4464};
	_Trigger->SetUp(EventZoneSize, TriggerLocation,
		std::move(StartEvent),
		std::move(EndEvent), std::move(EventQ));

	return _Trigger;
}

std::shared_ptr<object> Scene_Stage::Stage_6(std::weak_ptr<Player> _Player)
{
	auto _Trigger = object_mgr::instance().insert_object<Trigger>();

	std::weak_ptr<class object> wp_Trigger = _Trigger;

	// Start 
	auto StartEvent = [wp_Trigger, _Player]() {

		std::vector< std::weak_ptr<class object>> ReturnObjects;

		vec PrisonLocation = vec{ 3745,955};
		auto _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 0.95, Prison::EType::Ver,
			wp_Trigger, { 90,180 }, PrisonLocation);

		std::vector<vec> Locations =
		{
			{4377,776},{4500,1050},{4175,1066},{4373,1334}
		};

		ReturnObjects += Monster::TypeMatchMonstersSpawn<SwordMan>(_Player, std::move(Locations));

		Locations = { {4357,1189},{4362,998} };

		ReturnObjects += Monster::TypeMatchMonstersSpawn<ARCHER>(_Player, std::move(Locations));

		return ReturnObjects;
	};

	auto Event_1 = [_Player]()
	{
		std::vector< std::weak_ptr<class object>> ReturnObjects;
		
		std::vector<vec> Locations
		{
			{3944,561} , {4500,505} 
		};

		ReturnObjects += Monster::TypeMatchMonstersSpawn<SwordMan>(_Player, std::move(Locations));
		 Locations=
		{
			{4041,1046} , {4500,1056}
		};

		ReturnObjects += Monster::TypeMatchMonstersSpawn<ARCHER>(_Player, std::move(Locations));
		 Locations=
		{
			{3925,1484} , {4500,1519}
		};

		ReturnObjects += Monster::TypeMatchMonstersSpawn<WIZARD>(_Player, std::move(Locations));

		return ReturnObjects;
	};

	// 4351 1032
	std::queue<std::function<std::vector<std::weak_ptr<object>>()>> EventQ;
	EventQ.push(std::move(Event_1));
	std::pair<int, int>  EventZoneSize = { 200,200 };
	vec TriggerLocation = { 4141,997};
	std::pair<vec, vec> CameraRange = { { 4300,550} ,  { 4400,1300} };
	_Trigger->SetUp(EventZoneSize, TriggerLocation,
		std::move(StartEvent),
		[]{}, std::move(EventQ),true,
		std::move(CameraRange));

	return _Trigger;
}

std::shared_ptr<object> Scene_Stage::Stage_7(std::weak_ptr<Player> _Player)
{
	auto _Trigger = object_mgr::instance().insert_object<Trigger>();

	std::weak_ptr<class object> wp_Trigger = _Trigger;

	// Start 
	auto StartEvent = [wp_Trigger, _Player]() {

		std::vector< std::weak_ptr<class object>> ReturnObjects;

		vec PrisonLocation = vec{ 446,450};
		auto _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.5, 1, Prison::EType::Hor,
			wp_Trigger, { 200,120 }, PrisonLocation);

		 PrisonLocation = vec{ 446,950};
		_Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.5, 1, Prison::EType::Hor,
			wp_Trigger, { 200,120 }, PrisonLocation);

		 PrisonLocation = vec{ 230,685};
		 _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 1.8, Prison::EType::Ver,
			wp_Trigger, { 20,350 }, PrisonLocation);

		 PrisonLocation = vec{ 670,685 };
		 _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 1.8, Prison::EType::Ver,
			wp_Trigger, { 20,350 }, PrisonLocation);

		 PrisonLocation = vec{ 1246,450 };
		 _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.5, 1, Prison::EType::Hor,
			wp_Trigger, { 200,120 }, PrisonLocation);

		PrisonLocation = vec{ 1246,950 };
		_Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.5, 1, Prison::EType::Hor,
			wp_Trigger, { 200,120 }, PrisonLocation);

		PrisonLocation = vec{ 1030,685 };
		_Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 1.8, Prison::EType::Ver,
			wp_Trigger, { 20,350 }, PrisonLocation);

		PrisonLocation = vec{ 1470,685 };
		_Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 1.8, Prison::EType::Ver,
			wp_Trigger, { 20,350 }, PrisonLocation);

		std::vector<vec> Locations =
		{
			{769,400},{869,400},
			{769,500},{869,500},
			{769,600},{869,600}
		};

		ReturnObjects += Monster::TypeMatchMonstersSpawn<SwordMan>(_Player, std::move(Locations));
		Locations = 
		{
			{300,630},{400,630},{500,630},
			{1100,630},{1200,630},{1300,630}
		};
		ReturnObjects += Monster::TypeMatchMonstersSpawn<ARCHER>(_Player, std::move(Locations));

		return ReturnObjects;
	};

	auto Event_1 = [_Player]()
	{
		std::vector< std::weak_ptr<class object>> ReturnObjects;

		std::vector<vec> Locations =
		{
			{769,400},{869,400},
			{769,500},{869,500},
			{769,600},{869,600}
		};
		
		ReturnObjects += Monster::TypeMatchMonstersSpawn<SwordMan>
		(_Player, std::move(Locations));

		Locations =
		{
			{300,630},{400,630},
			{1100,630},{1200,630}
		};
		
		ReturnObjects += Monster::TypeMatchMonstersSpawn<WIZARD>(_Player, 
			std::move(Locations));
		
		return ReturnObjects;
	};

	auto EndEvent = [_Player]
	{
		std::vector<vec> Locations =
		{
			{250,300},{400,300},
			{550,300},{700,300}
		};
		
		Monster::TypeMatchMonstersSpawn<ARCHER>(_Player,
			std::move(Locations));
		 Locations =
		{
			{325,600},{475,600},
			{625,600},{775,600}
		};
		Monster::TypeMatchMonstersSpawn<SwordMan>(_Player,
				std::move(Locations));
			Locations =
		{
			{900,850},{1100,850},{1000,1000}
		};
		Monster::TypeMatchMonstersSpawn<WIZARD>(_Player,
					std::move(Locations));
		
		vec TelePort_Location = { 813,673};

		auto _Teleport = object_mgr::instance().insert_object<Teleport>();
		_Teleport->SetUp(TelePort_Location, true, ESceneID::EBoss);
	};
	
	std::queue<std::function<std::vector<std::weak_ptr<object>>()>> EventQ;
	EventQ.push(std::move(Event_1));
	std::pair<int, int>  EventZoneSize = { 200,200 };
	vec TriggerLocation = { 1296,654};
	
	_Trigger->SetUp(EventZoneSize, TriggerLocation,
		std::move(StartEvent),
		std::move(EndEvent), std::move(EventQ));

	return _Trigger;
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
