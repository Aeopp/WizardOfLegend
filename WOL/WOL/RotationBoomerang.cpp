#include "pch.h"
#include "RotationBoomerang.h"

void RotationBoomerang::initialize()
{
	Boomerang::initialize();

	ScaleX = 0.8f;
	ScaleY = 0.8f;


}

Event RotationBoomerang::update(float dt)
{
	Event _event = Boomerang::update(dt);

	ImgAngle += dt * RotationSpeed;

	return _event;
}

void RotationBoomerang::render(HDC hdc, vec camera_pos, vec size_factor)
{
	CalcImgAngle(ImgAngle);
	
	Boomerang::render(hdc, camera_pos, size_factor);
}
