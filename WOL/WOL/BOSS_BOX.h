#pragma once
#include "object.h"

class BOSS_BOX :
    public object
{
public :
	void initialize()override;
	Event update(float dt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	// 첫 생성 위치 생성 이후 런치하기까지 걸리는 시간 , 런치시작할 시점에 나아갈 방향을
	// 계산하기 위한 타겟
	void late_initialize(vec InitLocation, float InitDuration,
		std::weak_ptr<class object> wp_AttackTarget);
	void HitTile(RECT TileRt)override;

	std::weak_ptr< class collision_component> _collision_component;
	std::shared_ptr<class Bmp> sp_Bmp;
	std::shared_ptr<class Bmp> sp_SOILBmp;

	// 기둥공격 온 오프
	bool bLaunchPillar = false;
	void LaunchPillar(vec Dir );


	vec InitLocation{};

	uint32_t get_layer_id()const& override;

	float SOILAnimTick = 0;
	int SOILAnimRowDecrement = 0;

	
	
	// 서서히 떠오르는 효과
	float FlyTimeInAddY = 150;

	// 이펙트 컬럼 처음에 랜덤하게 지정해 생성
	int SOIL_NUM = 30;
	std::vector<std::pair<int32_t,vec>> SOIL_Cols;
	float SOIL_Duration = 3.f;
	static inline const float SOIL_AnimDelta = 0.08f;
	int SOIL_PaintSizeX;
	int SOIL_PaintSizeY;
	float SOIL_ScaleX;
	float SOIL_ScaleY;

	float Duration = 10.f;
	float Speed;
	int PaintSizeX;
	int PaintSizeY;
	float ScaleX;
	float ScaleY;

	float InitDuration;
	bool bLaunch{ false };
};


