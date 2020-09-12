#pragma once
#include "actor.h"


// 중간 보스가 활용 할 때에는
// 오너를 지정 안해주고
// 로테이션 센터를 미리 지정 해주는 식으로 하면 될듯
class Boomerang :
    public actor
{
public:
	void initialize()override;
	Event update(float dt)override;
	std::weak_ptr<class collision_component>  _collision_component;
	std::weak_ptr<class Bmp> wp_ResetImg{};	
	std::weak_ptr<class Bmp> wp_RotImg{};

	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	uint32_t get_layer_id()const& override;
	void Hit(std::weak_ptr<object> _target)override;
	void HitTile(RECT TileRt)override;
	
	std::array<POINT, 3> RotPts;

	void CalcImgAngle(float RotationImgDegree);
	virtual ~Boomerang()noexcept;
public:
	// 직선 방향 스피드
	float Duration;
	float AnimSpeed;
	int AnimRowIdx;
	int CurrentColIdx;
	float AnimDelta;
	float CurrentAnimRemainTime;
	static inline const float HitTileEffectTick = 0.15f;
	float HitTileEffectRemainTime = HitTileEffectTick;
};

