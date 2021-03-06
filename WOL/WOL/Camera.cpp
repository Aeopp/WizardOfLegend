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
	// 카메라가 고정되어 있는 상태라면 지정한 좌표에 카메라 고정
	if (bCameraFixing)
	{
		vec RangeLT = CameraFixPos.first;
		vec RangeRB = CameraFixPos.second;
		cp.x = std::clamp<>(cp.x, RangeLT.x, RangeRB.x);
		cp.y = std::clamp<>(cp.y, RangeLT.y, RangeRB.y); 
	}
		cp.x -= width;
		cp.y -= height;
	
	Shaking(dt);

	// 카메라흔들림
	cp += shake;
	shake = { 0,0 };

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
	Shake_Infos.push_back({ force, dir, duration });
}

void Camera::SetCameraFix(bool bFix, std::pair<vec, vec> CameraFixRange)
{
	bCameraFixing = bFix;
	this->CameraFixPos = CameraFixRange;
}

bool Camera::Shaking(float dt)
{
	std::erase_if(Shake_Infos, [dt](auto& ShakeInfo) {
		ShakeInfo.duration -= dt;
	return ShakeInfo.duration < 0; });

	for (auto& ShakeInfo: Shake_Infos)
	{	
		float  rand = math::Rand<float>({ -ShakeInfo.force  ,+ShakeInfo.force });
		shake +=  ShakeInfo.dir* rand;
	}

	return true;
};
