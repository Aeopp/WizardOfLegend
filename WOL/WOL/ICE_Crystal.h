#pragma once
#include "actor.h"

// �߰� ������ Ȱ�� �� ������
// ���ʸ� ���� �����ְ�
// �����̼� ���͸� �̸� ���� ���ִ� ������ �ϸ� �ɵ�
class ICE_Crystal :
    public actor
{
public:
	void initialize()override;
	Event update(float dt)override;
	std::weak_ptr<class collision_component>  _collision_component;
	uint32_t get_layer_id()const& override;

	static inline const float SoundTick = 0.5f; 
	float CurrentSoundTick = SoundTick;  
	float DistancefromCenterMin{ 80.f };
	float Tick{};
	float TickScale{ 1.f };
	float Duration{};
	// �߽����� �������� ȸ���ϴ� �ӵ�
	float RotationSpeedDegree{};
	float DistancefromCenter{};
	// �ݵ�� �����ؾ���
	vec CenterOfRotation{};
};