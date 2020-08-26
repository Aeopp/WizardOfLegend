#pragma once
#include "object.h"

class Camera : public  object
{
public:
	 Event update(float dt)override;
	 void initialize()override;

	vec shake{ 0,0 };

	class object_mgr* _object_mgr{ nullptr };
public:
	void camera_shake(float force, vec dir,float duration);
private:
	bool bShake{ false };

	float _force; 
	vec _dir;
	bool Shaking();
	bool Shaking_End();
};

