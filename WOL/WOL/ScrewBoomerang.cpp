#include "pch.h"
#include "ScrewBoomerang.h"

Event ScrewBoomerang::update(float dt)
{
	Event _event = actor::update(dt);

	Duration -= dt;
	if (Duration < 0)return Event::Die;

	CurrentRotationDir = 
	math::rotation_dir_to_add_angle( CurrentRotationDir, AngleSpeed * dt);

	RotationCenter+= _transform->_dir * _speed * dt;

	_transform->_location = RotationCenter + 
		(CurrentRotationDir * 
		RotationCenterDistance);

	ImgAngle += dt * RotationSpeed;

	ScaleX = 0.8f;
	ScaleY = 0.8f;



	return _event;
}

void ScrewBoomerang::late_initialize(vec RotationInitCenter, vec RotationInitDir)
{
	RotationCenter = std::move(RotationInitCenter);
	CurrentRotationDir = std::move(RotationInitDir);

	RotationSpeed = 900.f;

}
