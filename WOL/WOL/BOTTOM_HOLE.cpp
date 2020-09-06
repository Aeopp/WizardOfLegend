#include "pch.h"
#include "BOTTOM_HOLE.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
void BOTTOM_HOLE::initialize()
{
    object::initialize();

    // Y 소팅때문에

	//_transform->_location.x = FLT_MIN;
	//_transform->_location.y = FLT_MIN;

	sp_Bmp = Bmp_mgr::instance().Find_Image_SP(L"BOTTOM_HOLE");

	if (!sp_Bmp)return;
	
	ObjectTag = object::Tag::object;
	bAttacking = false;
	UniqueID = EObjUniqueID::None;

	ScaleX = 1.f;
	ScaleY = 1.f;
}

Event BOTTOM_HOLE::update(float dt)
{
    object::update(dt);

    Duration -= dt;
    if (Duration < 0)return Event::Die;

	return Event::None;
}

void BOTTOM_HOLE::render(HDC hdc, vec camera_pos, vec size_factor)
{
	vec DestLoc = PaintLocation- camera_pos;

	int PaintSizeX = 200;
	int PaintSizeY = 150;

	int WorldSizeX = (PaintSizeX * ScaleX);
	int WorldSizeY = (PaintSizeY * ScaleY);

	GdiTransparentBlt(hdc
		, DestLoc.x - WorldSizeX/2, DestLoc.y - WorldSizeY/2
		, WorldSizeX, WorldSizeY
		, sp_Bmp->Get_MemDC()
		,
		0, 0
		, PaintSizeX, PaintSizeY
		, COLOR::MRGENTA());
}

void BOTTOM_HOLE::late_initialize(vec InitLocation, float Duration)
{
	  _transform->_location = InitLocation;
    PaintLocation = InitLocation;
    this->Duration = Duration;
}

uint32_t BOTTOM_HOLE::get_layer_id() const&
{
    return layer_type::EMapDeco;
}
