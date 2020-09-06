#include "pch.h"
#include "Shadow.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "Color.h"
#include "actor.h"

void Shadow::ShadowImgInit()
{
	Bmp_mgr::instance().Insert_Bmp(L"NORMAL_SHADOW.bmp", L"NORMAL_SHADOW");
	Bmp_mgr::instance().Insert_Bmp(L"MIDDLE_SHADOW.bmp", L"MIDDLE_SHADOW");
	Bmp_mgr::instance().Insert_Bmp(L"BIG_SHADOW.bmp", L"BIG_SHADOW");
	Bmp_mgr::instance().Insert_Bmp(L"BIG_SHADOW_VER.bmp", L"BIG_SHADOW_VER");
}

std::shared_ptr<class Bmp> Shadow::FindImgByState()
{
	std::shared_ptr<Bmp> CurrentShadowImg{};

	switch (CurrentShadowState)
	{
	case EShadowState::MIDDLE:
		CurrentShadowImg = Bmp_mgr::instance().Find_Image_SP(L"MIDDLE_SHADOW");
		break;
	case EShadowState::NORMAL:
		CurrentShadowImg = Bmp_mgr::instance().Find_Image_SP(L"NORMAL_SHADOW");

		break;
	case EShadowState::BIG_VER:
		CurrentShadowImg = Bmp_mgr::instance().Find_Image_SP(L"BIG_SHADOW_VER");

		break;
	case EShadowState::BIG:
		CurrentShadowImg = Bmp_mgr::instance().Find_Image_SP(L"BIG_SHADOW");

		break;
	case EShadowState::MAX:
		break;
	default:
		break;
	}
	return CurrentShadowImg;
}

void Shadow::update()
{
	if (!bShadow)return;

	// 오너의 방향을 체크한다음 
	// 자기자신 쉐도우 정보를 변경 하기
	
	// 오너의 상태를 체크한다음 쉐도우 정보를 변경하기
}

void Shadow::initialize()
{

}

void Shadow::render(HDC hdc, vec camera_pos)
{
	if (!bShadow)return;

	auto sp_owner = _owner.lock();
	if (!sp_owner)return;

	vec dir = sp_owner->_transform->_dir.get_normalize();
	float d = dir.dot(vec{ +1,0 });

	if (abs(d) > math::cos45)
	{
		if (CurrentShadowState == EShadowState::BIG) {
			CurrentShadowState = EShadowState::BIG_VER;
		}
		
	}
	else
	{
		if (CurrentShadowState == EShadowState::BIG_VER) {
			CurrentShadowState = EShadowState::BIG;
		}
	};

	vec loc = sp_owner->_transform->_location;

	loc -= camera_pos;

	loc.x += correction.first;
	loc.y += correction.second;

	auto [px, py, wsx, wsy] = ShadowStates_Size[CurrentShadowState];
	auto [wscx, wscy] = world_size_correction;

	loc.x -= (wscx/2);
	loc.y -= (wscy / 2);

	loc.x -= (wsx / 2);
	loc.y -= (wsy / 2);

	auto Img = FindImgByState();
	if (!Img)return;

	HDC ShadowDC= Img->Get_MemDC();

	GdiTransparentBlt(hdc, loc.x, loc.y, wscx + wsx, wscy + wsy, ShadowDC
		, 0, 0, px, py, COLOR::MRGENTA());
}
