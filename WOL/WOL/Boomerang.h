#pragma once
#include "actor.h"


// �߰� ������ Ȱ�� �� ������
// ���ʸ� ���� �����ְ�
// �����̼� ���͸� �̸� ���� ���ִ� ������ �ϸ� �ɵ�
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

