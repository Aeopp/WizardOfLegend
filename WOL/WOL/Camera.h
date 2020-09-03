#pragma once
#include "object.h"

class Camera : public  object
{
public:
	 Event update(float dt)override;
	 void initialize()override;

	vec shake{ 0,0 };
	vec Rumble{ 0,0 };

	class object_mgr* _object_mgr{ nullptr };
public:
	void camera_shake(float force, vec dir,float duration);
	void camera_rumbling(float forceX,float forceY, float duration);

private:
	bool bShake{ false };
	bool bRumble{ false };

	float RX = 1;
	float RY = 1;

	float Tick = 0;

	float _force{};
	vec _dir{};
	bool Shaking( );
	bool Shaking_End( );
	bool Rumbling();
	bool Rumbling_End();
};

