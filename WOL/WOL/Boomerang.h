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
	std::weak_ptr<class collision_component>  _collision_component_lower;
	uint32_t get_layer_id()const& override;
public:
	float Duration;
	float RotationSpeedDegree;
	float DistancefromCenter;
	float TickScale;
	float Tick{};

	float Temp;
};

