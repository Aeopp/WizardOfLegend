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
	/// <param name="bFix"></param>
	/// 월드 기준으로 좌표를 패스해주세요.
	/// <param name="CameraFixRange"></param>
	void SetCameraFix(bool bFix, std::pair<vec, vec>CameraFixRange = {});
private:
	float Tick = 0;
	bool Shaking(float dt);
	bool bCameraFixing = false;
	std::pair<vec, vec> CameraFixPos{};
};

