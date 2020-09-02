#pragma once
#include "Boomerang.h"

// 중간 보스가 활용 할 때에는
// 오너를 지정 안해주고
// 로테이션 센터를 미리 지정 해주는 식으로 하면 될듯
class RotationBoomerang :
	public Boomerang
{
public:
	void initialize()override;
	Event update(float dt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	float ImgAngle = 0;
	float RotationSpeed = 1600.f;
};

