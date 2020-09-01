#pragma once
#include "actor.h"

// 중간 보스가 활용 할 때에는
// 오너를 지정 안해주고
// 로테이션 센터를 미리 지정 해주는 식으로 하면 될듯
class ICE_Crystal :
    public actor
{
public:
	void initialize()override;
	Event update(float dt)override;
	std::weak_ptr<class collision_component>  _collision_component_lower;
	uint32_t get_layer_id()const& override;

	float DistancefromCenterMin{ 80.f };
	float Tick{};
	float TickScale{ 1.f };
	float Duration{};
	// 중심점을 기준으로 회전하는 속도
	float RotationSpeedDegree{};
	float DistancefromCenter{};
	// 반드시 세팅해야함
	vec CenterOfRotation{};
};