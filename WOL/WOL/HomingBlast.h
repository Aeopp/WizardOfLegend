#pragma once
#include "object.h"
#include <deque>
// 생성시 반드시 타겟 지정
// 생성시 반드시 로케이션 지정
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

	float SoundTick = 0.1f;

	float ParticlesIdxPopTick = 0.2f;
	float CurrentAnimRemainTime = AnimDelta;
	// 인잇 타임이 끝나면 얼음기둥 으로 스프라이트 고정
	float AnimInitTime = 0.5f;
	int CurrentColIdx = 0;
	float Duration = 8;
	 // 디그리 기준 현재 목표방향으로의 회전 스피드 이 값은 틱마다 증가함
	float CurrentAngleSpeed;
	float HomingRotationAddSpeed;
	float ICE_EffectTick = 0.2f;

	float Speed;


	int PaintSizeX;
	int PaintSizeY;
	float ScaleX;
	float ScaleY;
};

