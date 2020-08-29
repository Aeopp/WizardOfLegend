#include "pch.h"
#include "Bmp_mgr.h"
#include "bmp.h"

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
