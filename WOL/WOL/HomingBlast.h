#pragma once
#include "ICE_Blast.h"
#include "object.h"
#include <deque>
// ������ �ݵ�� Ÿ�� ����
// ������ �ݵ�� �����̼� ����
class HomingBlast  : public object 
{
public :
	void initialize()override;
	Event update(float dt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void late_initialize(std::weak_ptr<object >Target, vec InitLocation, vec InitDir);
	std::weak_ptr< class collision_component> _collision_component;
	std::shared_ptr<class Bmp> sp_Bmp;
	uint32_t get_layer_id()const& override;
	void AnimUpdate();
	void ICE_EffectPlay();
	static inline float const AngleSpeedIncreaseRate = 45.f;
	static inline const float AnimDelta = 0.2f;

	std::deque<int> ParticlesColIdxs{ 2,2,2,2,2,2,2,2 };
	std::deque<vec> ParticlesPositions;
	static const inline float ParticlePosUpdateCycle = 0.1;
	float CurrentParticlePosUpdateDelta = ParticlePosUpdateCycle;
	float HomingRotationCurrentSpeed;


	float ParticlesIdxPopTick = 0.2f;
	float CurrentAnimRemainTime = AnimDelta;
	// ���� Ÿ���� ������ ������� ���� ��������Ʈ ����
	float AnimInitTime = 0.5f;
	int CurrentColIdx = 0;
	float Duration = 10.f;
	 // ��׸� ���� ���� ��ǥ���������� ȸ�� ���ǵ� �� ���� ƽ���� ������
	float CurrentAngleSpeed;
	float HomingRotationAddSpeed;
	
	float Speed;


	int PaintSizeX;
	int PaintSizeY;
	float ScaleX;
	float ScaleY;
};

