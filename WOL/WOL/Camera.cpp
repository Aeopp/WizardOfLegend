#include "pch.h"
#include "Camera.h"
#include "object_mgr.h"
#include "game.h"

void Camera::update(float dt)
{
	object::update(dt);

	auto _target_ptr = _owner.lock();
	if (!_target_ptr)return;
	if (!_object_mgr)return;

	RECT _rt = game::instance().client_rect;

	int width =  (_rt.right - _rt.left) / 2;
	int height = (_rt.bottom - _rt.top) / 2;

	_object_mgr->camera_pos = this->_transform->_location = _target_ptr->_transform->_location;

	_object_mgr->camera_pos.x -= width;
	_object_mgr->camera_pos.y -= height;
}

void Camera::initialize()
{
	object::initialize();

	_object_mgr = &object_mgr::instance();
}
