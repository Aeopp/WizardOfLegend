#pragma once
#include "Effect.h"
class SwordManAttack :
    public Effect
{
public:
	float AnimDuration;

	float DefaultAttackDuration;
	float CurrentAttackDuration;
	math::EDir _Dir;
	bool bRender = false;
	void EffectStart(vec dir,vec location);
	Event update(float dt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void initialize()override;
	std::weak_ptr<class collision_component>_collision_component;
	void Hit(std::weak_ptr<object> _target)override;
};

