#pragma once
#include "Effect.h"

enum AttackNumber
{
	_1,
	_2,
};

class EffectPlayerAttack :
    public Effect
{
public:
	std::weak_ptr<class collision_component> _collision_component;

	void AttackReady(float Duration,
		float AnimDuration, float PushForce,vec InitLocation,
		AttackNumber _Num,float AnimAngle);
	
	float Angle{};
	// 1 or 2
	 // bool bAttacking{ false } ;
	bool bRender{ false };

	float AnimT = 0;

	AttackNumber CurrentAttackNum {};
	Event update(float dt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void initialize()override;
	void Hit(std::weak_ptr<class object> wp_Target)override;
	float SoundTick = 0.05f;
private:
	void AttackEnd();
};

	/*void late_initialize(int ImgLocationX, int ImgLocationY,
		std::wstring ImgKey, layer_type layer_ID, int AnimColNum,
		int AnimRowIndex, float Duration, float AnimDuration,
		int PaintSizeX, int PaintSizeY, float ScaleX, float ScaleY);*/




