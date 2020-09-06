#pragma once
#include "object.h"

struct shake_Info
{
	float force; 
	vec dir; 
	float duration;
};

class Camera : public  object
{
public:
	 Event update(float dt)override;
	 void initialize()override;
	class object_mgr* _object_mgr{ nullptr };
public:
	void camera_shake(float force, vec dir,float duration);
	std::vector<shake_Info> Shake_Infos{};
	vec shake{ 0,0 };
private:
	float Tick = 0;
	bool Shaking(float dt);
};

