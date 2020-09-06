#pragma once
#include "object.h"

class BOSS_SKILL :public object
{
public:
	void initialize()override;
	Event update(float dt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void HitTile(RECT rt)override;
	// 첫 생성 위치 생성 이후 런치하기까지 걸리는 시간 , 런치시작할 시점에 나아갈 방향을
	// 계산하기 위한 타겟
	void late_initialize(vec InitLocation);
	std::weak_ptr< class collision_component> _collision_component;
	std::shared_ptr<class Bmp> sp_Bmp;
	std::shared_ptr<class Bmp> sp_SOILBmp;

	float Tick = 0;
	float fFloationgY = 0;
	// 흙 이펙트는 정해진 컬럼에서 ROW 감소시키며 진행
	int SOILEffectNum = 8;
	std::vector<std::pair<int,vec>> MySOILColIdxs;
	int CurrentSOILRow = 6;
	static inline const float SOIL_AnimDelta = 0.07f;
	float CurrentSOIL_AnimDelta = SOIL_AnimDelta; 
	uint32_t get_layer_id()const& override;

	static inline const float PillarAnimDelta = 0.1f;
	float CurrentPillarAnimDelta = PillarAnimDelta; 
	int CurrentPillarColIdx = 0;
	// 이펙트 컬럼 처음에 랜덤하게 지정해 생성
	
	float Pillar_ScaleX;
	float Pillar_ScaleY;
	float SOIL_ScaleX;
	float SOIL_ScaleY;

	float Duration = 10.f;
};

