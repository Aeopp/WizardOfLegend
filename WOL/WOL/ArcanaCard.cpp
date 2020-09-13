#include "pch.h"
#include "collision_component.h"
#include "collision_mgr.h"
#include "ArcanaCard.h"
#include "Bmp.h"
#include "Bmp_mgr.h"
#include "game.h"
#include "sound_mgr.h"

#include "UIInventory.h"

void ArcanaCard::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!_transform)return;
	if (!sp_Img)return;

	int PaintSizeX = 128; 
	int PaintSizeY = 192; 

	vec DestLoc = _transform->_location;
	DestLoc -= camera_pos;
	DestLoc -= vec{ PaintSizeX,PaintSizeY } *0.5f;

	GdiTransparentBlt(hdc, DestLoc.x, DestLoc.y, PaintSizeX, PaintSizeY,
		sp_Img->Get_MemDC(), 0, 0, PaintSizeX, PaintSizeY, COLOR::MRGENTA());
}

void ArcanaCard::initialize()
{
	object::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr,collision_tag::EArcana,
		ERect);
	auto sp_collision  = _collision_component.lock();
	if (!sp_collision)return;

	sp_collision->bCollision = true;
	sp_collision->bCollisionSlideAnObject = false;
	sp_collision->bCollisionTargetPushFromForce = false;
	sp_collision->bHitEffect = true;
	sp_collision->bRender = true;
	sp_collision->bSlide = true;
	sp_collision->bSuperArmor = true;
	sp_collision->_size = { 100,100 };
}

void ArcanaCard::late_initialize(vec Location,ESkill SetSkill, std::wstring ImgKey)
{
	if (!_transform)return;
	_transform->_location = Location;
	this->_Skill = SetSkill;
	sp_Img = Bmp_mgr::instance().Find_Image_SP(ImgKey);
}

void ArcanaCard::Hit(std::weak_ptr<object> _target)
{
	object::Hit(_target);

	auto sp_Target =_target.lock();
	if (!sp_Target)return;

	if (sp_Target->ObjectTag== object::Tag::player)
	{
		for (auto& [key, _SlotInfo] : game::SlotInfoMap)
		{
			if (_SlotInfo._Skill == _Skill)
			{
				SOUNDPLAY("GET_SKILL", 1.f, false);
				_SlotInfo.bAcquire = true;
			}
		}

		bDie = true;
	}
}
