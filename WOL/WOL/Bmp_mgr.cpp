#include "pch.h"
#include "Bmp_mgr.h"
#include "bmp.h"
#include "Shadow.h"

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
