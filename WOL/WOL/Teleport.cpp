#include "pch.h"
#include "Input_mgr.h"

#include "Scene_mgr.h"
#include "Teleport.h"
#include "sound_mgr.h"
#include "collision_component.h"
#include "collision_mgr.h"
#include "Bmp.h"
#include "Bmp_mgr.h"


void Teleport::render(HDC hdc, vec camera_pos, vec size_factor)
{
	object::render(hdc, camera_pos, size_factor);
	if (!_transform)return;
	if (!sp_Bmp)return;
	vec DestLoc = _transform->_location;
	DestLoc -= camera_pos;
	vec PaintSize = { 311,298 };
	DestLoc -= (PaintSize *0.5);

	GdiTransparentBlt(hdc, DestLoc.x, DestLoc.y, PaintSize.x,
		PaintSize.y, sp_Bmp->Get_MemDC(), 0, 0, 311, 298, COLOR::MRGENTA());
}

void Teleport::initialize()
{
	object::initialize();
	sp_Bmp = Bmp_mgr::instance().Find_Image_SP(L"TELEPORT");
	
	SOUNDPLAY("teleport", 1.f, false);

	UniqueID = EObjUniqueID::Portal;
}


void Teleport::SetUp(vec Location, bool bEnding)
{
	if (!_transform)return;
	_transform->_location = std::move(Location);
	this->bEnding = bEnding;
	if (this->bEnding)
	{
		_collision_component = collision_mgr::instance().
			insert(_ptr, collision_tag::ETrigger, figure_type::ERect);

		auto sp_Portal = _collision_component.lock();
		if (!sp_Portal) return;

		sp_Portal->bCollision = true;
		sp_Portal->bCollisionSlideAnObject = false;
		sp_Portal->bCollisionTargetPushFromForce = false;
		sp_Portal->bHitEffect = false;
		sp_Portal->bRender = true;
		sp_Portal->bSlide = false;
		sp_Portal->bSuperArmor = true;
		sp_Portal->PushForce = 0.f;
		sp_Portal->_size = { 150,150};
	}
}

uint32_t Teleport::get_layer_id() const&
{
	return layer_type::EMapDeco;
}
