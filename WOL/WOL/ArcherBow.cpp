#include "pch.h"
#include "ArcherBow.h"
#include "ArcherArrow.h"
#include "collision_component.h"
#include "collision_mgr.h"
#include "object_mgr.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "player.h"
#include "player_info.h"


void ArcherBow::initialize()
{
	object::initialize();

	BowScaleX = 0.8f;
	BowScaleY = 0.8f;

	sp_Img = Bmp_mgr::instance().Find_Image_SP(L"ARCHER_BOW_LEFT");
	
	if (!sp_Img)return;

	CurrentBowAnimDelta = DefaultBowAnimDelta = 3.f / 4.f;

}
Event ArcherBow::update(float dt)
{
	Event _Event = object::update(dt);

	vec dir = _transform->_dir;

	if (bPreparation)
	{
		auto sp_Target = wp_Target.lock();
		if (!sp_Target)return Event::None;
		

		if (dir.x < 0)
		{
			sp_Img = Bmp_mgr::instance().Find_Image_SP(L"ARCHER_BOW_LEFT");
			  CurrentBowRow = BowSpriteAngleTable.first[math::AngleFromVec(dir) / 20];
		}
		else
		{
			sp_Img = Bmp_mgr::instance().Find_Image_SP(L"ARCHER_BOW_RIGHT");
		 CurrentBowRow = BowSpriteAngleTable.second[math::AngleFromVec(dir) / 20];
		}
	
		CurrentBowAnimDelta -= dt;
		if (CurrentBowAnimDelta < 0)
		{
			++CurrentBowCol;
			CurrentBowAnimDelta = DefaultBowAnimDelta;
			if (CurrentBowCol > 3)
			{
				CurrentBowCol = 0;
			}
		}
	}

	return _Event;
};

void ArcherBow::HitTile(RECT rt)
{
	object::HitTile(rt);
	end();
};

uint32_t ArcherBow::get_layer_id() const&
{
	return layer_type::EEffect;
}
void ArcherBow::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!sp_Img) return;

	auto sp_owner = _owner.lock();
	if (!sp_owner)return;
	if (!sp_owner->_transform)return;

	if(bPreparation)
	{
		vec owner_loc = sp_owner->_transform->_location;

		vec v = owner_loc - camera_pos;
		v.x -= (BowPaintSizeX / 2);
		v.y -= (BowPaintSizeY / 2);
		vec bow_correction{ 0,0 };
		v -= bow_correction;
		GdiTransparentBlt(hdc, v.x, v.y, 
			BowPaintSizeX*BowScaleX, 
			BowPaintSizeY*BowScaleY,
			sp_Img->Get_MemDC(), CurrentBowCol*BowPaintSizeX, 
			CurrentBowRow* BowPaintSizeY,
			BowPaintSizeX, BowPaintSizeY, RGB(0, 255, 255));
	}
}
void ArcherBow::Hit(std::weak_ptr<object> _target)
{
	object::Hit(_target);

	auto sp_Target = _target.lock()	;
	if (!sp_Target)return;

	if ((sp_Target->id !=object::ID::player) )return;

	auto sp_Player = std::dynamic_pointer_cast<Player>(sp_Target);

	if (!sp_Player)return;
	
	sp_Player->_player_info->AddHp(-Attack);
};

void ArcherBow::Preparation(bool Set)
{
	bPreparation = Set;
	CurrentBowCol = 0;
	CurrentBowAnimDelta = 3.f / 4.1f;
}

void ArcherBow::launch(vec init, vec dir)
{
	if (!_transform)return;
	dir = dir.get_normalize();
	// ¿ÞÂÊ ¿À¸¥ÂÊ
	int ArrowRow = 0;
	if (dir.x < 0)
	{
		sp_Img = Bmp_mgr::instance().Find_Image_SP(L"ARCHER_BOW_LEFT");
		ArrowRow = CurrentBowRow = BowSpriteAngleTable.first[math::AngleFromVec(dir) / 20];
	}
	else
	{
		sp_Img = Bmp_mgr::instance().Find_Image_SP(L"ARCHER_BOW_RIGHT");
		ArrowRow = CurrentBowRow = BowSpriteAngleTable.second[math::AngleFromVec(dir) / 20];
	}
	auto Arrow = object_mgr::instance().insert_object<ArcherArrow>();

	Arrow->launch(init,dir, ArrowRow, sp_Img);
	
	_transform->_dir = std::move(dir);
	CurrentBowAnimDelta = 3.f / 4.1f;
	CurrentBowCol = 3;
	bPreparation = false;
};

void ArcherBow::end()
{
	CurrentBowCol = 0;
	CurrentBowAnimDelta = 3.f / 4.1f;
	bPreparation = false;
}
