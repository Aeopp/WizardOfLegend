#include "pch.h"
#include "Freezing_Interface.h"
#include "collision_component.h"
#include "Color.h"
#include "Bmp_mgr.h"
#include "object_mgr.h"
#include "Bmp.h"



void Freezing_Interface::Freezing_update(float dt, std::weak_ptr<collision_component>
                                         _collision_component)
{
	_Freezing_Info.update(dt);
	
	if (_Freezing_Info.IsFreezing())
	{
		auto sp_collision = _collision_component.lock();
		if (!sp_collision)return;
		sp_collision->bSuperArmor = true;
	}
	else
	{
		auto sp_collision = _collision_component.lock();
		if (!sp_collision)return ;
		sp_collision->bSuperArmor = false;
	}
}

void Freezing_Interface::Freezing_render(HDC hdc, vec location)
{
	_Freezing_Info.render(hdc, location, render_size);
}

bool Freezing_Interface::probability_calculation(float _Percentage)
{
	float DICE = math::Rand<float>({ 0.01f,1.f });
	return ! (DICE >= _Percentage);
}


std::shared_ptr<Bmp> Freezing_Interface::Freezing::GetImg()
{
	static auto sp_Img = Bmp_mgr::instance().Find_Image_SP(L"ICE_BLAST");
	return sp_Img;
}

void Freezing_Interface::Freezing::Freez(vec msgLocation)
{
	if (RemainTime > 0)return;

	object_mgr::instance().TextEffectMap[Freezing_Interface::EffectColor].
		push_back({ msgLocation,vec{0,1.f},2.5,30,L"얼어붙음!" });

	RemainTime = Duration;
	ColIdx = 0;
	AnimTick = AnimDelta;
}


bool Freezing_Interface::Freezing::update(float dt)
{
	RemainTime -= dt;
	if (RemainTime < 0)return false;

	AnimTick -= dt;
	if (AnimTick < 0)
	{
		AnimTick = AnimDelta;
		++ColIdx;
		ColIdx = min(ColIdx, 8);
		if (RemainTime > Duration-InitAnimTime)
		{
			ColIdx = min(ColIdx, 2);
		}
	}
	return true;
}

void Freezing_Interface::Freezing::render(HDC hdc, vec location, vec size)
{
	if (!IsFreezing())return;

	auto sp_Img = GetImg();
	if (!sp_Img)return;

	vec DestLoc = location - (size * 0.5f);

	GdiTransparentBlt(hdc, DestLoc.x, DestLoc.y, size.x, size.y,
		sp_Img->Get_MemDC(), PaintSize.x * ColIdx, 0,
		PaintSize.x, PaintSize.y, COLOR::MRGENTA());
}

