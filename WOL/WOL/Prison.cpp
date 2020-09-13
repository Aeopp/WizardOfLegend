#include "pch.h"
#include "Prison.h"
#include "collision_component.h"
#include "collision_mgr.h"
#include "Bmp.h"
#include "Bmp_mgr.h"
#include "Trigger.h"
#include "sound_mgr.h"


Event Prison::update(float dt)
{
	auto sp_Owner = _owner.lock();
	if (!sp_Owner)return Event::Die;

	auto sp_Trigger = std::dynamic_pointer_cast<Trigger>(sp_Owner);
	if (!sp_Trigger)return Event::Die;

	if(sp_Trigger->bEventEnd)
	{
		SOUNDPLAY("PRISON_END", 1.f, false);
		return Event::Die;
	}

	Event _Event = object::update(dt);
	return _Event;
}

void Prison::render(HDC hdc, vec camera_pos, vec size_factor)
{
	object::render(hdc, camera_pos, size_factor);

	if (!sp_Img)return;

	vec DestPaintSize = { PaintSizeX * ScaleX, PaintSizeY * ScaleY };
	vec DestLoc = _transform->_location - camera_pos - (DestPaintSize * 0.5);

	GdiTransparentBlt(hdc,
		DestLoc.x, DestLoc.y
		, DestPaintSize.x, DestPaintSize.y
		, sp_Img->Get_MemDC(),
		0,
		 0,
		PaintSizeX, PaintSizeY,
		COLOR::MRGENTA());
}

void Prison::initialize()
{
	object::initialize();

	wp_collision = collision_mgr::instance().
		insert(_ptr, collision_tag::EPrison, figure_type::ERect);

	auto sp_collision = wp_collision.lock();
	if (!sp_collision) return;

	sp_collision->bCollision = true;
	sp_collision->bCollisionSlideAnObject = true;
	sp_collision->bCollisionTargetPushFromForce = false;
	sp_collision->bHitEffect = false;
	sp_collision->bRender = true;
	sp_collision->bSlide = false;
	sp_collision->PushForce = 0.f;
	sp_collision->_size = { 50,50 };
	sp_collision->bSuperArmor = true;

	this->bAttacking = false;

	ScaleX = ScaleY = 1.f;

	UniqueID = EObjUniqueID::Prison;
	ObjectTag = Tag::object;


}

void Prison::release()
{
	object::release();
}

void Prison::SetUp(float ScaleX, float ScaleY, 
Prison::EType _Type, std::weak_ptr<class object> _owner,
std::pair<int,int> CollisionSize,vec Location )
{
	if (!_transform)return;
	_transform->_location = std::move(Location); 
	this->ScaleX = ScaleX;
	this->ScaleY = ScaleY;
	this->_owner = std::move(_owner);

	auto sp_collision = wp_collision.lock();
	if (!sp_collision) return;
	sp_collision->_size = CollisionSize;

	switch (_Type)
	{
	case Prison::EType::Hor:
		sp_Img = Bmp_mgr::instance().Find_Image_SP(L"PRISON_HOR");
		PaintSizeX = 292;
		PaintSizeY = 283; 
		break;
	case Prison::EType::Ver:
		sp_Img = Bmp_mgr::instance().Find_Image_SP(L"PRISON_VER");
		PaintSizeX = 23;
		PaintSizeY = 427;
		break;
	default:
		break;
	}


	SOUNDPLAY("PRISON_START", 1.f, false);
}

void Prison::SetCollisionCorrection(vec Correction)
{
	auto sp_collision = wp_collision.lock();
	if (!sp_collision)return;
	sp_collision->correction = Correction;
}
