#include "pch.h"
#include "ScrewBoomerang.h"
#include "collision_component.h"
#include "sound_mgr.h"
#include "collision_mgr.h"

Event ScrewBoomerang::update(float dt)
{
	Event _event = actor::update(dt);

	HitTick -= dt;

	Duration -= dt;
	if (Duration < 0)return Event::Die;

	CurrentRotationDir = 
	math::rotation_dir_to_add_angle( CurrentRotationDir, AngleSpeed * dt);

	RotationCenter+= _transform->_dir * _speed * dt;

	_transform->_location = RotationCenter + 
		(CurrentRotationDir * 
		RotationCenterDistance);

	ImgAngle += dt * RotationSpeed;

	return _event;
}

void ScrewBoomerang::late_initialize(vec RotationInitCenter, vec RotationInitDir)
{
	RotationCenter = std::move(RotationInitCenter);
	CurrentRotationDir = std::move(RotationInitDir);

	RotationSpeed = 900.f;

	Duration = 5.f;

	ScaleX = 0.8f;
	ScaleY = 0.8f;

	PaintSizeX = 200;
	PaintSizeY = 200;
	// 직선 운동 속도
	_speed = 1500.f;
	AnimSpeed = 2.f;

	auto sp_collision = _collision_component.lock();
	if (!sp_collision)return;
	sp_collision->bTileHitEffect = false;

}

void ScrewBoomerang::HitTile(RECT TileRt)
{
	actor::HitTile(TileRt);

	vec dir = _transform->_dir;
	dir *= -1;

	RAND_SOUNDPLAY("WALL_HITTED_FIREDRAGON", { 0,2 }, 1.f, false);

	int RandAngle = math::Rand<int>({ -45,+45 });

	dir = math::rotation_dir_to_add_angle(dir, RandAngle);

	_transform->_dir = dir;

	if (HitTick < 0)
	{
		HitTick = 0.3f;

		collision_mgr::instance().HitEffectPush(_transform->_location, 0.3f);
	}
	CalcImgAngle(math::radian_to_degree(atan2f(dir.y, dir.x)));
}
