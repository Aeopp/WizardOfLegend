#pragma once
#include "actor.h"
class ICE_Blast :
    public actor
{
public:
	void initialize()override;
	//void render(HDC hdc, vec camera_pos, vec size_factor)override;
	//void late_initialize(vec _location);
	Event update(float dt)override;
	std::weak_ptr<class collision_component>  _collision_component;
	void HitTile(RECT _rt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void IceEffectPlay();

	float ICE_EffectTick = 0.2f;
	float SoundTick = 0.5f;
	uint32_t get_layer_id()const& override;

	
	float Duration;

	bool bSuccess{};
};

