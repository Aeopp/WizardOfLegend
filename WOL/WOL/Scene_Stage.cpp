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

			auto _Teleport = obj_mgr.insert_object<Teleport>();
			_Teleport->SetUp(PlayerSpawnLocation, false);

		

		auto boomerang_card = obj_mgr.insert_object<ArcanaCard>(PlayerSpawnLocation
			+ vec{ 0 ,+200 }, ESkill::BOOMERANG, L"BOOMERANG_CARD");


		manage_objs.push_back(boomerang_card);
	 // 	manage_objs.push_back(crystal_card);
		//manage_objs.push_back(fire_card);
	 // 	manage_objs.push_back(blast_card);
		
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

void Scene_Stage::TriggerSetUp(std::weak_ptr<class Player> _Player)
{
	auto _Trigger = object_mgr::instance().insert_object<Trigger>();

	std::weak_ptr<class object> wp_Trigger = _Trigger;

	// Start 
	auto StartEvent = [wp_Trigger,_Player]() {

		std::vector< std::weak_ptr<class object>  > ReturnObjects;

		vec PrisonLocation = vec{ 1350,2450 };
		vec SwordManLocation = vec{ 1000, 2500 };
		
		auto _Prison = object_mgr::instance().insert_object<Prison>();
		_Prison->SetUp(1.f, 1.f, Prison::EType::Hor,
			wp_Trigger, { 100,100 }, PrisonLocation);

		auto _Swordman= object_mgr::instance().insert_object<SwordMan>
		(_Player,	vec{ 1000,2500 });
		
		ReturnObjects.push_back(_Swordman);
		return ReturnObjects;
	};
	// 1 
	auto Event_1 = [_Player]()
	{
		std::vector< std::weak_ptr<class object>> ReturnObjects;
		
		vec FireCardLocation =  { 1000, 2500 };
		
		auto fire_card = object_mgr::instance().
		insert_object<ArcanaCard>(FireCardLocation, ESkill::FIRE, L"FIRE_DRAGON_CARD");

		std::vector<vec> MonsterLocation { {1000,2500} , { 1300,2500 }  };

		for(const auto& locaion  :MonsterLocation)
		{
			auto sp_Monster = object_mgr::instance().insert_object<SwordMan>
				(_Player, locaion);
			
			ReturnObjects.push_back(sp_Monster);
		}
		
		return ReturnObjects;
	};
	// 2 
	auto Event_2 = [_Player]()
	{
		std::vector< std::weak_ptr<class object>  > ReturnObjects;

		vec BlastCardLocation = vec{ 1000,2500 };
		
		auto blast_card = object_mgr::instance().
		insert_object<ArcanaCard>(BlastCardLocation	, ESkill::BLAST, L"ICE_BLAST_CARD");
		
		std::vector<vec> MonsterLocation{ {1000,2500} , { 1300,2500 } };

		for (const auto& locaion : MonsterLocation)
		{
			auto sp_Monster = object_mgr::instance().insert_object<ARCHER>
				(_Player, locaion);

			ReturnObjects.push_back(sp_Monster);
		}
		
		return ReturnObjects;
	};
	// 3
	auto Event_3 = [_Player]()
	{
		std::vector< std::weak_ptr<class object>  > ReturnObjects;

		vec CrystalCardLocation = vec{ 1000,2500 };

		auto crystal_card = object_mgr::instance().
		insert_object<ArcanaCard>(CrystalCardLocation,ESkill::CRYSTAL, L"ICE_KRYSTAL_CARD");

		std::vector<vec> MonsterLocation{ {1000,2500} , { 1300,2500 } };

		for (const auto& locaion : MonsterLocation)
		{
			auto sp_Monster = object_mgr::instance().insert_object<WIZARD>
				(_Player, locaion);
			
			ReturnObjects.push_back(sp_Monster);
		}

		return ReturnObjects;
	};
	

	std::queue<std::function<std::vector<std::weak_ptr<object>>()>> EventQ;
	EventQ.push(std::move(Event_1));
	EventQ.push(std::move(Event_2 ));
	EventQ.push(std::move(Event_3));
	vec  EventZoneSize = { 200,200 };
	vec TriggerLocation = { 1350,2450 };
	
	std::pair<vec, vec> camera_range = { {1350,2450} , {1500,2600}  };
	_Trigger->SetUp({ 200,200 }, vec{ 1350,2450 },
		std::move(StartEvent),
		[]() {}, std::move(EventQ));
	
	manage_objs.push_back(_Trigger);
}
