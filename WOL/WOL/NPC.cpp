#include "pch.h"
#include "NPC.h"
#include "Bmp_mgr.h"

#include "Bmp.h"

void NPC::render(HDC hdc, vec camera_pos, vec size_factor)
{
	object::render(hdc, camera_pos, size_factor);


	if (!_transform)return;
	if (!sp_NpcImg)return;
	if (!sp_TableImg)return;
	_Shadow.render(hdc, camera_pos);
	int PaintSizeX = 128;
	int PaintSizeY = 197;

	vec DestLoc = _transform->_location;
	DestLoc -= camera_pos;
	DestLoc -= vec{ PaintSizeX,PaintSizeY } *0.5f;

	GdiTransparentBlt(hdc, DestLoc.x, DestLoc.y, PaintSizeX, PaintSizeY,
		sp_NpcImg->Get_MemDC(), 0, 0, PaintSizeX, PaintSizeY, COLOR::MRGENTA());

	DestLoc += vec{ PaintSizeX,PaintSizeY } *0.5f;;

	DestLoc += vec{ 0, PaintSizeY * 0.6f +  234/2};

	PaintSizeX = 606;
	PaintSizeY = 234;

	DestLoc -= (vec{ PaintSizeX , PaintSizeY }  *0.5f);

	GdiTransparentBlt(hdc, DestLoc.x, DestLoc.y, PaintSizeX, PaintSizeY,
		sp_TableImg->Get_MemDC(), 0, 0, PaintSizeX, PaintSizeY, COLOR::MRGENTA());


}

void NPC::initialize()
{
	object::initialize();

	sp_TableImg = Bmp_mgr::instance().Find_Image_SP(L"SHOP_TABLE");
	sp_NpcImg = Bmp_mgr::instance().Find_Image_SP(L"ITEMSHOP_NPC");

	_Shadow.bShadow = true;
	_Shadow.CurrentShadowState = EShadowState::MIDDLE;
	_Shadow._owner = _ptr;
	_Shadow.world_size_correction = { 50,20};
	_Shadow.correction = { 18,100 };
}

void NPC::SetUp(vec Location)
{
	if (!_transform)return;
	_transform->_location = std::move(Location);

}
