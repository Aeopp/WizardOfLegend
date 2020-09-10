#include "pch.h"
#include "UIInventory.h"
#include "render_component.h"
#include "game.h"
#include "Color.h"
#include "UISlot.h"
#include "object_mgr.h"
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

	std::vector<std::tuple<std::wstring,std::wstring,ESkill>> IconKeyTable
	{
		{L"",L"NORMAL_ATTACK_EX",ESkill::Normal},
		{L"",L"DASH_EX",ESkill::Dash},
		{L"FIRE_DRAGON_ICON",L"FIRE_DRAGON_EX",ESkill::FIRE},
		{L"ICE_BLAST_ICON",L"ICE_BLAST_EX",ESkill::BLAST},
		{L"ICE_KRYSTAL_ICON",L"ICE_KRYSTAL_EX",ESkill::CRYSTAL},
		{L"GAIA_ARMOR_ICON",L"GAIA_ARMOR_EX",ESkill::ARMOR},
		{L"BOOMERANG_ICON",L"",ESkill::BOOMERANG}
	};

	for (int i = 0; i < IconKeyTable.size()-1; ++i)
	{
		SlotInfo _SlotInfo;
		_SlotInfo.Location = vec{ 270 + 66* i,160 };
		auto& [ImgKey, InfoKey, _ESkill] = IconKeyTable[i];

		_SlotInfo.ImgKey = std::move(ImgKey);
		_SlotInfo.InfoKey = std::move(InfoKey);
		_SlotInfo._Skill = _ESkill;
		SlotInfoMap[i] = std::move(_SlotInfo);
	}
	
	// 마지막 창작스킬 부메랑 슬롯
	int IDX = IconKeyTable.size() - 1;
	SlotInfo _SlotInfo;
	_SlotInfo.Location = vec{ 270 + 66 * (IDX-1)-2,224 };
	auto& [ImgKey, InfoKey, _ESkill] = IconKeyTable[IDX];

	_SlotInfo.ImgKey = std::move(ImgKey);
	_SlotInfo.InfoKey = std::move(InfoKey);
	_SlotInfo._Skill = _ESkill;
	SlotInfoMap[IDX] = std::move(_SlotInfo);


	SlotInfoMap[0].bAcquire = true;
	SlotInfoMap[1].bAcquire = true;
}

void UIInventory::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!bInventoryOpen)return;

	UI::render(hdc, camera_pos, size_factor);

	for (auto& [slotIdx,_SlotInfo]: SlotInfoMap)
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

	if (!SlotInfoMap[CurrentInfoIdx].bAcquire)return;
	vec DestLoc = SlotInfoMap[CurrentInfoIdx].Location;

	DestLoc -= SlotInfo::SelectSize * 0.5;

	auto sp_SelectIcon = Bmp_mgr::instance().Find_Image_SP(L"SELECT_ICON");
	HDC CurrentSelectDC = sp_SelectIcon->Get_MemDC();

	GdiTransparentBlt(hdc, DestLoc.x, DestLoc.y,
		SlotInfo::SelectSize.x, SlotInfo::SelectSize.y, CurrentSelectDC,
		0, 0, 61, 61, COLOR::MRGENTA());

	if (!SlotInfoMap[CurrentInfoIdx].bAcquire)return ;

	vec InfoDescLoc = vec{ 235,514 };
	auto sp_CurrentInfoBmp = Bmp_mgr::instance().Find_Image_SP(
		SlotInfoMap[CurrentInfoIdx].InfoKey);

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

	for (auto& [SlotIdx, _SlotInfo]  :SlotInfoMap)
	{
		//if (!SlotInfoMap[SlotIdx].bAcquire)return Event::None;
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
					std::swap(SlotInfoMap[SlotIdx].ImgKey, SlotInfoMap[CurrentSelectIdx].ImgKey);
					std::swap(SlotInfoMap[SlotIdx].InfoKey, SlotInfoMap[CurrentSelectIdx].InfoKey);
					std::swap(SlotInfoMap[SlotIdx]._Skill, SlotInfoMap[CurrentSelectIdx]._Skill);
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
