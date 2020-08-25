#include "pch.h"
#include "object.h"


void object::update(float dt)
{

}

void object::render(HDC hdc,vec camera_pos)
{

}

void object::initialize()
{
	_transform = std::make_shared<Transform>();
	_transform->_location = vec{ 0,0 };

}

void object::release()
{

}

uint32_t object::get_layer_id() const&
{
	return layer_type::EObject;
};

void object::set_ptr(std::weak_ptr<object> _ptr)&
{
	this->_ptr = std::move(_ptr);
}
void object::Hit(std::weak_ptr<object> _target)
{
	auto _ptr = _target.lock();
	if (!_ptr)	return;
};

