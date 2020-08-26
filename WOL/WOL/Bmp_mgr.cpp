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
};

void Bmp_mgr::release()
{

}

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

	return {};
}

std::weak_ptr<Bmp> Bmp_mgr::Find_Image(std::wstring ImageKey)
{
	using namespace std;

	auto iter = bmp_map.find(ImageKey);

	if (bmp_map.end() == iter)
		return {};

	return iter->second;
}

void Bmp_mgr::Release()
{
	bmp_map.clear();
}
