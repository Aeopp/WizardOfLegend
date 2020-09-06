#include "pch.h"
#include "BOSS_SKILL.h"
#include "object_mgr.h"
#include "collision_component.h"
#include "collision_mgr.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "BOTTOM_HOLE.h"

void BOSS_SKILL::initialize()
{
	object::initialize();

	MySOILColIdxs.reserve(SOILEffectNum);
	std::generate_n(std::back_inserter(MySOILColIdxs), SOILEffectNum,
		[]() {
			auto SoilRenderPosCorrection = std::pair<int, vec>{ math::Rand<int>({ 0,3 }),vec{math::Rand<int>({-30,30}),80} };
			return      SoilRenderPosCorrection; });
	


	// 흙 이펙트는 정해진 컬럼에서 ROW 감소시키며 진행
	CurrentSOILRow = 6;
	CurrentPillarColIdx = 0;
	// 이펙트 컬럼 처음에 랜덤하게 지정해 생성

	Pillar_ScaleX = 1;
	Pillar_ScaleY = 1;
	SOIL_ScaleX = 1;
	SOIL_ScaleY = 1;

	Duration = 5.f;

	_collision_component =
		collision_mgr::instance().insert(_ptr, collision_tag::EMonsterAttack,
			ERect);

	auto sp_collision = _collision_component.lock();
	if (!sp_collision)return;

	sp_collision->bCollisionTargetPushFromForce = true;
	sp_collision->PushForce = 30.f;
	sp_collision->correction = {-30,-80};
	sp_collision->_size = { 35,85 };
	sp_collision->bRender = true;
	sp_collision->bSlide = false;
	sp_collision->bCollisionSlideAnObject = false;
	sp_collision->bSuperArmor = true;

	bAttacking = true;

	sp_Bmp = Bmp_mgr::instance().Find_Image_SP(L"BOSS_SKILL");
	sp_SOILBmp = Bmp_mgr::instance().Find_Image_SP(L"SOIL_EFFECT");

 	if (!sp_Bmp)return;
	if (!sp_SOILBmp)return;

	ObjectTag = object::Tag::monster_attack;

	Attack = { 50,60 };

	UniqueID = EObjUniqueID::BOSS_Pillar;


}

Event BOSS_SKILL::update(float dt)
{
	if (bDie)return Event::Die;


	object::update(dt);

	Duration -= dt;
	CurrentPillarAnimDelta -= dt;

	if (Duration < 0)
	{
		return Event::Die;
	}

	Tick += dt * 4;

	fFloationgY = -(1 - std::cosf(Tick)) * 200;

	// 지속 시간이  얼마 안남았을때에 흙이펙트 애니메이션
	if (/*Duration < 1.f*/true )
	{
		CurrentSOIL_AnimDelta -= dt;
		if (CurrentSOIL_AnimDelta < 0)
		{
			CurrentSOIL_AnimDelta = SOIL_AnimDelta;
			CurrentSOILRow = max(CurrentSOILRow - 1, 0);
		}
	}

	if (CurrentPillarAnimDelta < 0)
	{
		CurrentPillarAnimDelta = PillarAnimDelta;
		CurrentPillarColIdx = min(CurrentPillarColIdx + 1, 2);
	}

	return Event::None;
}

void BOSS_SKILL::render(HDC hdc, vec camera_pos, vec size_factor)
{
    // 지속시간이 얼마 안남았을때에는 렌더링 기둥에서 흙으로 교체하고
    // ROW 감소시키며 애니메이션 진행하기
	if (!_transform)return;
	if (!sp_Bmp)return;

	if (Duration > 0.75)
	{
		vec DestLoc = (_transform->_location - camera_pos);
		int WorldSizeX = Pillar_ScaleX * 80;
		int WorldSizeY = Pillar_ScaleY * 255;

		GdiTransparentBlt(hdc
			, DestLoc.x - WorldSizeX, DestLoc.y - WorldSizeY
			, WorldSizeX, WorldSizeY
			, sp_Bmp->Get_MemDC()
			,
			CurrentPillarColIdx * 80, 0
			, 80, 255
			, COLOR::MRGENTA());
	}
	if (Duration > 3.5)
	{
		int PillarBottomY = (_transform->_location.y - camera_pos.y) - (255*Pillar_ScaleY)/2;
		int SOIL_WorldSizeY = SOIL_ScaleY * 60; 
		int SOIL_WorldSizeX = SOIL_ScaleX * 60;

		int PillarDestY = PillarBottomY - SOIL_WorldSizeY/3; 
		int PillarDestX = (_transform->_location.x - camera_pos.x) - SOIL_WorldSizeX;

		for (int i = 0; i < SOILEffectNum; ++i)
		{
			GdiTransparentBlt(hdc
				, PillarDestX+ MySOILColIdxs[i].second.x, PillarDestY+ MySOILColIdxs[i].second.y +fFloationgY
				, SOIL_WorldSizeX, SOIL_WorldSizeY
				, sp_SOILBmp->Get_MemDC()
				,
				MySOILColIdxs[i].first *60, CurrentSOILRow*60
				, 60, 60
				, COLOR::MRGENTA());


			PillarDestY -= SOIL_WorldSizeY / 3;
		}
	}
}

void BOSS_SKILL::HitTile(RECT rt)
{
	bDie = true;
}

void BOSS_SKILL::late_initialize(vec InitLocation)
{
    if (!_transform)return;

    _transform->_location = std::move(InitLocation);

	auto sp_Hole = object_mgr::instance().insert_object<BOTTOM_HOLE>(_transform->_location + vec{ -45,-35 }, Duration + 5.f);
}


uint32_t BOSS_SKILL::get_layer_id() const&
{
	return layer_type::EObject;
};

