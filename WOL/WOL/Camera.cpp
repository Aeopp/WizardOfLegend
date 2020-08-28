#include "pch.h"
#include "Camera.h"
#include "object_mgr.h"
#include "game.h"
#include "Input_mgr.h"
#include "timer.h"

Event Camera::update(float dt)
{
	Event _E = object::update(dt);

	auto _target_ptr = _owner.lock();
	if (!_target_ptr)return Event::None;
	if (!_object_mgr)return Event::None;

	RECT _rt = game::instance().client_rect;

	int width =  (_rt.right - _rt.left) / 2;
	int height = (_rt.bottom - _rt.top) / 2;

	vec& cp = _object_mgr->camera_pos;

	cp = _transform->_location = _target_ptr->_transform->_location;

	cp.x -= width;
	cp.y -= height;

	if(bShake)
	cp += shake;

	return _E;
}

void Camera::initialize()
{
	object::initialize();

	_object_mgr = &object_mgr::instance();

	shake = vec{ 0,0 };
};

void Camera::camera_shake(float force, vec dir,float duration  =0.05f)
{
	_force = force;
	_dir = dir;
	bShake = true;

	Timer::instance().event_regist(time_event::ERemaingWhile, duration, &Camera::Shaking, this);
	Timer::instance().event_regist(time_event::EOnce, duration+0.005f, &Camera::Shaking_End, this);
}

bool Camera::Shaking()
{
	float  rand = math::Rand_N<float >({ -_force,+_force});

	shake = _dir* rand;

	return true;
};

bool Camera::Shaking_End()
{
	shake = vec{ 0,0 };
	bShake = false;

	return true;
};
