#include "pch.h"
#include "object.h"

Event object::update(float dt)
{
	return Event::None;
}

void object::render(HDC hdc, vec camera_pos, vec size_factor)
{

};

void object::initialize()
{
	_transform = std::make_shared<Transform>();
	_transform->_location = vec{ 0,0 };
	_transform->_dir = vec{ 0,+1 };
	UniqueID = EobjUniqueID::None;

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
};

void object::Hit(std::weak_ptr<object> _target)
{
};

void object::HitTile(RECT TileRt)
{

};

