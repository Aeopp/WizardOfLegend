#include "pch.h"
#include "Bmp_mgr.h"
#include "bmp.h"
#include "Shadow.h"
#include <filesystem>

Bmp_mgr::~Bmp_mgr()noexcept
{
	release();
};

void Bmp_mgr::initialize()
{
	DefaultPath = L"..\\..\\Resources\\WOL_TEXTURE\\";

	// 이미지들 대신 이니셜라이즈
	ImageSelectMap[ETileSelect::Info_1] = { Insert_Bmp(L"MAPTILE_1.bmp", L"MAPTILE_1"),L"MAPTILE_1" };
	ImageSelectMap[ETileSelect::Info_2] = { Insert_Bmp(L"MAPTILE_2.bmp", L"MAPTILE_2"),L"MAPTILE_2" };
	ImageSelectMap[ETileSelect::Info_3] = { Insert_Bmp(L"MAPTILE_3.bmp", L"MAPTILE_3"),L"MAPTILE_3" };
	ImageSelectMap[ETileSelect::Info_4] = { Insert_Bmp(L"MAPTILE_#.bmp", L"MAPTILE_#"),L"MAPTILE_#" };

	DecoTileInfoMap[EDecoTileInfo::statue] = { 0,0,145,284 };
	DecoTileInfoMap[EDecoTileInfo::flag] = { 146,0,291,214 };
	DecoTileInfoMap[EDecoTileInfo::pillar] = { 302,13,428,350 };
	DecoTileInfoMap[EDecoTileInfo::chair_H] = { 0,295,218,435 };
	DecoTileInfoMap[EDecoTileInfo::chair_V] = { 219,242,291,510 };
	DecoTileInfoMap[EDecoTileInfo::pool_12] = { 0,438,145,510 };
	DecoTileInfoMap[EDecoTileInfo::pool_22] = { 292,365,437,510 };

	namespace fs = std::filesystem;

	//for (const fs::directory_entry& entry :
	   // fs::directory_iterator(fs::current_path() / L".." / L".." / L"Resources" / L"WOL_TEXTURE")) 
	//{
	   // auto filename = entry.path().wstring();
	   // auto key = entry.path().stem().wstring();

	   // Insert_Bmp(filename, key);
	//}
	Shadow::ShadowImgInit();

	Insert_Bmp(DefaultPath + L"Back.bmp", L"Back");
	Insert_Bmp(DefaultPath + L"Back.bmp", L"BackBuffer");
	Insert_Bmp(DefaultPath + L"GAIA_ARMOR.bmp", L"GAIA_ARMOR");
	Insert_Bmp(DefaultPath + L"ICE_CRYSTAL.bmp", L"ICE_CRYSTAL");
	Insert_Bmp(DefaultPath + L"BOTTOM_HOLE.bmp", L"BOTTOM_HOLE");
	Insert_Bmp(DefaultPath + L"SUMMON.bmp", L"SUMMON");
	Insert_Bmp(DefaultPath + L"ICE_BLAST.bmp", L"ICE_BLAST");

	Insert_Bmp(DefaultPath + L"SWORDMAN_ATTACK.bmp", L"SWORDMAN_ATTACK");
	Insert_Bmp(DefaultPath + L"WOL_NORMAL_ATTACK.bmp", L"WOL_NORMAL_ATTACK");
	Insert_Bmp(DefaultPath + L"SUMMON_CARD_ARCHER.bmp", L"SUMMON_CARD_ARCHER");
	Insert_Bmp(DefaultPath + L"SUMMON_CARD_SWORDMAN.bmp", L"SUMMON_CARD_SWORDMAN");
	Insert_Bmp(DefaultPath + L"SUMMON_CARD_WIZARD.bmp", L"SUMMON_CARD_WIZARD");
	Insert_Bmp(DefaultPath + L"SUMMON_CARD_WIZARDBALL.bmp", L"SUMMON_CARD_WIZARDBALL");
	Insert_Bmp(DefaultPath + L"FIRE_PARTICLE.bmp", L"FIRE_PARTICLE");
	Insert_Bmp(DefaultPath + L"HITEFFECT.bmp", L"HITEFFECT");

	Insert_Bmp(DefaultPath + L"MONEY.bmp", L"MONEY");
	Insert_Bmp(DefaultPath + L"SKILL_FIREDRAGON_COM.bmp", L"SKILL_FIREDRAGON_COM");
	Insert_Bmp(DefaultPath + L"FIRE_PARTICLE.bmp", L"FIRE_PARTICLE");
	Insert_Bmp(DefaultPath + L"WOL_NORMAL_ATTACK.bmp", L"WOL_NORMAL_ATTACK");

	Insert_Bmp(DefaultPath + L"SWORDMAN_RIGHT.bmp", L"SWORDMAN_RIGHT");
	Insert_Bmp(DefaultPath + L"SWORDMAN_ATTACK.bmp", L"SWORDMAN_ATTACK");
	Insert_Bmp(DefaultPath + L"__SWORDMAN_ATTACK.bmp", L"__SWORDMAN_ATTACK");


	Insert_Bmp(DefaultPath + L"HITEFFECT.bmp", L"HITEFFECT");

	Insert_Bmp(DefaultPath + L"ARCHER_BOW_LEFT.bmp", L"ARCHER_BOW_LEFT");
	Insert_Bmp(DefaultPath + L"ARCHER_BOW_RIGHT.bmp", L"ARCHER_BOW_RIGHT");

	Insert_Bmp(DefaultPath + L"ARCHER_RIGHT.bmp", L"ARCHER_RIGHT");
	Insert_Bmp(DefaultPath + L"ARCHER_LEFT.bmp", L"ARCHER_LEFT");
	Insert_Bmp(DefaultPath + L"SWORDMAN_ATTACK_RESET.bmp", L"SWORDMAN_ATTACK_RESET");
	Insert_Bmp(DefaultPath + L"SWORDMAN_ATTACK_ROTATION.bmp", L"SWORDMAN_ATTACK_ROTATION");

	Insert_Bmp(DefaultPath + L"WIZARD_LEFT.bmp", L"WIZARD_LEFT");
	Insert_Bmp(DefaultPath + L"WIZARD_RIGHT.bmp", L"WIZARD_RIGHT");

	Insert_Bmp(DefaultPath + L"WIZARD_FIRE.bmp", L"WIZARD_FIRE");
	Insert_Bmp(DefaultPath + L"WIZARD_BALL.bmp", L"WIZARD_BALL");
	Insert_Bmp(DefaultPath + L"SUMMON_CARD_WIZARDBALL.bmp", L"SUMMON_CARD_WIZARDBALL");

	Insert_Bmp(DefaultPath + L"MIDDLE_BOSS_LEFT.bmp", L"MIDDLE_BOSS_LEFT");
	Insert_Bmp(DefaultPath + L"MIDDLE_BOSS_RIGHT.bmp", L"MIDDLE_BOSS_RIGHT");
	Insert_Bmp(DefaultPath + L"BOSS_LEFT.bmp", L"BOSS_LEFT");
	Insert_Bmp(DefaultPath + L"BOSS_RIGHT.bmp", L"BOSS_RIGHT");
	Insert_Bmp(DefaultPath + L"BOSS_ATTACK.bmp", L"BOSS_ATTACK");
	Insert_Bmp(DefaultPath + L"BOSS_SKILL.bmp", L"BOSS_SKILL");

	Insert_Bmp(DefaultPath + L"MIDDLEBOSS_NAMEBAR.bmp", L"MIDDLEBOSS_NAMEBAR");
	Insert_Bmp(DefaultPath + L"BOSS_NAMEBAR.bmp", L"BOSS_NAMEBAR");

	Insert_Bmp(DefaultPath + L"ICE_BLAST.bmp", L"ICE_BLAST");
	Insert_Bmp(DefaultPath + L"SOIL_EFFECT.bmp", L"SOIL_EFFECT");
	Insert_Bmp(DefaultPath + L"BOSS_BOXATTACK.bmp", L"BOSS_BOXATTACK");

	Insert_Bmp(DefaultPath + L"PRISON_VER.bmp", L"PRISON_VER");
	Insert_Bmp(DefaultPath + L"PRISON_HOR.bmp", L"PRISON_HOR");

	Insert_Bmp(DefaultPath + L"INVENTORY.bmp", L"INVENTORY");

	Insert_Bmp(DefaultPath + L"INVENTORY.bmp", L"INVENTORY");

	Insert_Bmp(DefaultPath + L"ICE_KRYSTAL_ICON.bmp", L"ICE_KRYSTAL_ICON");
	Insert_Bmp(DefaultPath + L"ICE_BLAST_ICON.bmp", L"ICE_BLAST_ICON");
	Insert_Bmp(DefaultPath + L"GAIA_ARMOR_ICON.bmp", L"GAIA_ARMOR_ICON");
	Insert_Bmp(DefaultPath + L"FIRE_DRAGON_ICON.bmp", L"FIRE_DRAGON_ICON");

	Insert_Bmp(DefaultPath + L"NORMAL_ATTACK_EX.bmp", L"NORMAL_ATTACK_EX");
	Insert_Bmp(DefaultPath + L"ICE_KRYSTAL_EX.bmp", L"ICE_KRYSTAL_EX");
	Insert_Bmp(DefaultPath + L"ICE_BLAST_EX.bmp", L"ICE_BLAST_EX");
	Insert_Bmp(DefaultPath + L"GAIA_ARMOR_EX.bmp", L"GAIA_ARMOR_EX");
	Insert_Bmp(DefaultPath + L"FIRE_DRAGON_EX.bmp", L"FIRE_DRAGON_EX");
	Insert_Bmp(DefaultPath + L"DASH_EX.bmp", L"DASH_EX");

	Insert_Bmp(DefaultPath + L"SELECT_ICON.bmp", L"SELECT_ICON");


	Insert_Bmp(DefaultPath + L"BOOMERANG_ICON.bmp", L"BOOMERANG_ICON");
	Insert_Bmp(DefaultPath + L"BOOMERANG_SKILLBAR.bmp", L"BOOMERANG_SKILLBAR");
	Insert_Bmp(DefaultPath + L"BOOMERANG_CARD.bmp", L"BOOMERANG_CARD");

	Insert_Bmp(DefaultPath + L"ICE_KRYSTAL_CARD.bmp", L"ICE_KRYSTAL_CARD");
	Insert_Bmp(DefaultPath + L"ICE_BLAST_CARD.bmp", L"ICE_BLAST_CARD");
	Insert_Bmp(DefaultPath + L"FIRE_DRAGON_CARD.bmp", L"FIRE_DRAGON_CARD");

	Insert_Bmp(DefaultPath + L"GAIA_ARMOR_CARD.bmp", L"GAIA_ARMOR_CARD");
	Insert_Bmp(DefaultPath + L"SHOP_TABLE.bmp", L"SHOP_TABLE");
	Insert_Bmp(DefaultPath + L"ITEMSHOP_NPC.bmp", L"ITEMSHOP_NPC");
	Insert_Bmp(DefaultPath + L"POTION.bmp", L"POTION");

	Insert_Bmp(DefaultPath + L"TELEPORT.bmp", L"TELEPORT");

};



void Bmp_mgr::release()
{
	
};

std::weak_ptr<Bmp> Bmp_mgr::Insert_Bmp(std::wstring FileName, std::wstring ImageKey)
{
	using namespace std;
	
	auto iter = bmp_map.find(ImageKey);

	FileName = DefaultPath + FileName;

	if (bmp_map.end() == iter)
	{
		auto sp_bmp = std::shared_ptr<Bmp>(new Bmp);

		sp_bmp->Load_Bmp(FileName);

		auto iter = bmp_map.emplace( ImageKey, std::move(sp_bmp) );

		return iter.first->second;
	}
	else
	{
		return iter->second;
	}
	return {};
}

std::weak_ptr<Bmp> Bmp_mgr::Find_Image_WP(std::wstring ImageKey)
{
	using namespace std;

	auto iter = bmp_map.find(ImageKey);

	if (bmp_map.end() == iter)
		return {};

	return iter->second;
};

std::shared_ptr<Bmp> Bmp_mgr::Find_Image_SP(std::wstring ImageKey)
{
	using namespace std;

	auto iter = bmp_map.find(ImageKey);

	if (bmp_map.end() == iter)
		return {};

	return iter->second;
};


void Bmp_mgr::Release()
{
	bmp_map.clear();
}
