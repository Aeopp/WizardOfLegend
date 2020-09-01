#pragma once
#include "Effect.h"
class SwordManAttack :
    public Effect
{
public:
	math::EDir _Dir;
	bool bRender = false;
	void EffectStart(float duration,vec dir,vec location);
	Event update(float dt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void initialize()override;
};

