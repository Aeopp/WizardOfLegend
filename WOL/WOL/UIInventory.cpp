#include "pch.h"
#include "UIInventory.h"
#include "render_component.h"
#include "game.h"
#include "Color.h"
#include "UISlot.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "Input_mgr.h"
#include "sound_mgr.h"

void UIInventory::initialize()
{
	UI::initialize();

	// 해상도 변경시 비율 조정을 위해서
	auto [x, y] = game::instance().size_factor();

	_render_component = render_component::
	LoadRenderComponent_SP(L"INVENTORY.bmp", L"INVENTORY");
	_render_component->Default_Src_Paint_Size = vec{ 576 * x,865* y };
	_render_component->_ColorKey = COLOR::MRGENTA();
	_render_component->_Img_src = RECT{ 0,0,576,865};
	_render_component->_RenderDesc = ERender::Transparent;

	_transform->_location = vec{ 400,380};
	Scale = 0.85;

	////TODO DEBUG CODE Remove Plz
	//SlotInfoMap[2].bAcquire = true;
	//SlotInfoMap[3].bAcquire = true;
	//SlotInfoMap[4].bAcquire = true;
	//SlotInfoMap[5].bAcquire = true;
	//SlotInfoMap[6].bAcquire = true;
}

void UIInventory::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!bInventoryOpen)return;

	UI::render(hdc, camera_pos, size_factor);

	for (auto& [slotIdx,_SlotInfo]: game::SlotInfoMap)
	{
		if (!_SlotInfo.bAcquire)continue;
		vec DestLoc = _SlotInfo.Location;
		DestLoc -= SlotInfo::Size*0.5;
		
		auto sp_CurrentSlotBmp = Bmp_mgr::instance().Find_Image_SP(_SlotInfo.ImgKey);
		if (!sp_CurrentSlotBmp)continue;

		HDC CurrentSlotImg = sp_CurrentSlotBmp->Get_MemDC();

		GdiTransparentBlt(hdc, DestLoc.x, DestLoc.y,
			SlotInfo::Size.x, SlotInfo::Size.y, CurrentSlotImg,
			0, 0, 52, 52, COLOR::MRGENTA());
	}

	if (!game::SlotInfoMap[CurrentInfoIdx].bAcquire)return;
	vec DestLoc = game::SlotInfoMap[CurrentInfoIdx].Location;

	DestLoc -= SlotInfo::SelectSize * 0.5;

	auto sp_SelectIcon = Bmp_mgr::instance().Find_Image_SP(L"SELECT_ICON");
	HDC CurrentSelectDC = sp_SelectIcon->Get_MemDC();

	GdiTransparentBlt(hdc, DestLoc.x, DestLoc.y,
		SlotInfo::SelectSize.x, SlotInfo::SelectSize.y, CurrentSelectDC,
		0, 0, 61, 61, COLOR::MRGENTA());

	if (!game::SlotInfoMap[CurrentInfoIdx].bAcquire)return ;

	vec InfoDescLoc = vec{ 235,514 };
	auto sp_CurrentInfoBmp = Bmp_mgr::instance().Find_Image_SP(
		game::SlotInfoMap[CurrentInfoIdx].InfoKey);

	if (!sp_CurrentInfoBmp)return;
	GdiTransparentBlt(hdc,InfoDescLoc.x,InfoDescLoc.y,
		403.75, 170, 
	sp_CurrentInfoBmp->Get_MemDC(), 0, 0, 472, 200, COLOR::MRGENTA());
}	


uint32_t UIInventory::get_layer_id() const&
{
	return UIInfo::get_layer_id();
}

Event UIInventory::update(float dt)
{
	if (!bInventoryOpen)return Event::None;
	ChangeCoolTime -= dt;

	Event _Event = UIInfo::update(dt);

	auto oPos = Input_mgr::instance().GetWindowMousePos();
	if (!oPos)return Event::None;

	vec MousePos = *oPos;

	for (auto& [SlotIdx, _SlotInfo]  : game::SlotInfoMap)
	{
	
		if (!_SlotInfo.bAcquire)return Event::None;

		RECT _SlotRect = { _SlotInfo.Location.x - 24 ,
			_SlotInfo.Location.y - 24 ,
			_SlotInfo.Location.x + 24,
			_SlotInfo.Location.y + 24 };

		bool bOnMouse = math::RectInPoint(_SlotRect, MousePos);

		if (bOnMouse)
		{
			CurrentInfoIdx = SlotIdx;

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				if (ChangeCoolTime<0 && bSelect)
				{
					std::swap(game::SlotInfoMap[SlotIdx].ImgKey, game::SlotInfoMap[CurrentSelectIdx].ImgKey);
					std::swap(game::SlotInfoMap[SlotIdx].InfoKey, game::SlotInfoMap[CurrentSelectIdx].InfoKey);
					std::swap(game::SlotInfoMap[SlotIdx]._Skill, game::SlotInfoMap[CurrentSelectIdx]._Skill);
					SOUNDPLAY("CHANGE_SKILL", 1.f, false);

					ChangeCoolTime = 0.1f;
					bSelect = false;
					return Event::None;
				}
				else if (ChangeCoolTime<0)
				{
					SOUNDPLAY("CHANGE_SKILL", 1.f, false);
					bSelect = true;
					CurrentSelectIdx = SlotIdx;
					ChangeCoolTime = 0.1f;
					return Event::None;
				}
		
			}
		}
	}
	return _Event;
}
