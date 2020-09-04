#pragma once
#include "ICE_Crystal.h"
class BossCrystal :
    public ICE_Crystal
{
public :
public:
	void initialize()override;
	void late_initialize(vec RotationCenter,float RotationDuration, 
						std::weak_ptr<class object> wp_AttackTarget, float AttackSpeed);
	void Hit(std::weak_ptr<class object>wp_Target)override;

	Event update(float dt)override;
	uint32_t get_layer_id()const& override;
	vec RotationCenter;
	vec AttackDir{};
	float AttackSpeed;
	float RotationDuration;
	bool bAttack = false;
	void CalcAttackDir();
};

