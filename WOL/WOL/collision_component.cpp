#include "pch.h"
#include "collision_component.h"
#include "object.h"

void collision_component::set_owner(std::weak_ptr<object> _owner)&
{
	this -> _owner = std::move(_owner);
}

void collision_component::Hit(std::weak_ptr<class object> _target)
{
	auto _ptr = _owner.lock();
	if (!_ptr)return;

	_ptr->Hit(_target);
}

RECT collision_component::make_rect()
{
	auto _ptr = _owner.lock();
	if (!_ptr)return RECT{};

	RECT _rt;
	vec _loc = _ptr->_transform->_location;

	_rt.left = _loc.x - _size.x;
	_rt.right= _loc.x  + _size.x;
	_rt.top = _loc.y - _size.y ;
	_rt.bottom = _loc.y +_size.y ;

	return _rt;
}

circle collision_component::make_circle()
{
	auto _ptr = _owner.lock();
	if (!_ptr)return circle{};

	return { _ptr->_transform->_location ,_size.x };
}


