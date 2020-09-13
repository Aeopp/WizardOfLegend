#include "pch.h"
#include "game.h"
#include "Font.h"
#include "Scene_mgr.h"
#include "timer.h"
#include "sound_mgr.h"
#include "Bmp_mgr.h"
#include "object_mgr.h"
#include "collision_mgr.h"
#include "player_info.h"
#include "Bmp.h"

void game::render()
{
	Bmp_mgr& bmp_mgr =Bmp_mgr::instance();

	auto SP_Back =bmp_mgr.Find_Image_SP(L"Back");
	if (!SP_Back)return;

	auto SP_BackBuffer = bmp_mgr.Find_Image_SP(L"BackBuffer");
	if (!SP_BackBuffer)return;

	static HDC HMemDC; HDC HBackBuffer;

	HMemDC = SP_Back->Get_MemDC();
	HBackBuffer = SP_BackBuffer->Get_MemDC();

	BitBlt(HBackBuffer, 0, 0, client_rect.right, client_rect.bottom, HMemDC, 0, 0, SRCCOPY);

	CurrentHdc = HBackBuffer;

	if (bDebug)
	{
		{
			std::wstring msg = bFrameLimit ? L"FrameLimit ON " : L"FrameLimit OFF";
			Font(CurrentHdc, RGB(111, 111, 111), 0, 450, 20, msg);
		}

		{
			std::wstring msg = bDeltaTimeUnFixed ? L"DeltaTimeUnFixed ON " : L"DeltaTimeUnFixed OFF";
			Font(CurrentHdc, RGB(111, 111, 111), 0, 350, 20, msg);
		}
	}

	Scene_mgr::instance().render(HBackBuffer, size_factor());
	

	BitBlt(hDC, 0, 0, client_rect.right, client_rect.bottom, HBackBuffer, 0, 0, SRCCOPY);

	CurrentHdc = hDC;
}

void game::update()
{
	Scene_mgr::instance().update(DeltaTime);
}

void game::initialize()
{
	hDC = GetDC(hWnd);

	_player_info = std::make_shared<player_info>();
	SlotDataInit();
	
	Scene_mgr::instance().initialize();

	Scene_mgr::instance().Scene_Change(ESceneID::EStart);
}

void game::release()
{
	Scene_mgr::instance().release();

	Bmp_mgr::instance().release();

	sound_mgr::instance().Release();

	object_mgr::instance().release();

	collision_mgr::instance().release();

	ReleaseDC(hWnd, CurrentHdc);
}

void game::late_update()
{

}

void game::debug_cul_obj_setup(int cul_obj, int render_obj)
{
	print_cul_obj = cul_obj;
	print_render_obj = render_obj;
}


std::pair<float, float> game::size_factor()
{
	return {
		(float)(client_rect.right - client_rect.left) / width  ,
		 (float)(client_rect.bottom - client_rect.top) / height };
}

void game::SlotDataInit()
{
	std::vector<std::tuple<std::wstring, std::wstring, ESkill>> IconKeyTable
	{
		{L"",L"NORMAL_ATTACK_EX",ESkill::Normal},
		{L"",L"DASH_EX",ESkill::Dash},
		{L"FIRE_DRAGON_ICON",L"FIRE_DRAGON_EX",ESkill::FIRE},
		{L"ICE_BLAST_ICON",L"ICE_BLAST_EX",ESkill::BLAST},
		{L"ICE_KRYSTAL_ICON",L"ICE_KRYSTAL_EX",ESkill::CRYSTAL},
		{L"GAIA_ARMOR_ICON",L"GAIA_ARMOR_EX",ESkill::ARMOR},
		{L"BOOMERANG_ICON",L"",ESkill::BOOMERANG}
	};

	for (int i = 0; i < IconKeyTable.size() - 1; ++i)
	{
		SlotInfo _SlotInfo;
		_SlotInfo.Location = vec{ 270 + 66 * i,160 };
		auto& [ImgKey, InfoKey, _ESkill] = IconKeyTable[i];

		_SlotInfo.ImgKey = std::move(ImgKey);
		_SlotInfo.InfoKey = std::move(InfoKey);
		_SlotInfo._Skill = _ESkill;
		game::SlotInfoMap[i] = std::move(_SlotInfo);
	}

	// 마지막 창작스킬 부메랑 슬롯
	int IDX = IconKeyTable.size() - 1;
	SlotInfo _SlotInfo;
	_SlotInfo.Location = vec{ 270 + 66 * (IDX - 1) - 2,224 };
	auto& [ImgKey, InfoKey, _ESkill] = IconKeyTable[IDX];

	_SlotInfo.ImgKey = std::move(ImgKey);
	_SlotInfo.InfoKey = std::move(InfoKey);
	_SlotInfo._Skill = _ESkill;
	game::SlotInfoMap[IDX] = std::move(_SlotInfo);

	game::SlotInfoMap[0].bAcquire = true;
	game::SlotInfoMap[1].bAcquire = true;

	game::SlotInfoMap[2].bAcquire = true;
	game::SlotInfoMap[3].bAcquire = true;
	game::SlotInfoMap[4].bAcquire = true;
	game::SlotInfoMap[5].bAcquire = true;
	game::SlotInfoMap[6].bAcquire = true;
}
