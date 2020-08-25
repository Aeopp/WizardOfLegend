#include "pch.h"
#include "object.h"
#include "game.h"


void object::initialize()
{
	set_b_update_location_from_owner(true);
	set_distance_from_owner({ 0,0 });

	_collision_component = std::shared_ptr<collision_component>{};
	_collision_component->setpos(vec(get_location()));
	_collision_component->setsize(vec(get_size()));
	_collision_component->set_owner(std::weak_ptr<object>(get_ptr()));
	_collision_component->set_tag(figure_type::ERect);

};

void object::render(HDC hdc)
{
	const float _size_x = _size.x / 2;
	const float _size_y = _size.x / 2;

	switch (_render_type_id)
	{
	case render_type::none:
		break;
	case render_type::rect:
		Rectangle(hdc, _location.x- _size_x, _location.y-_size_y, _location.x + _size_x, _location.y + _size_y);
		break;
	case render_type::circle:
		Ellipse(hdc, _location.x - _size_x, _location.y - _size_y, _location.x + _size_x, _location.y + _size_y);
		break;
	default:
		break;
	}
};

void object::Hit(std::weak_ptr<object> _obj_weak)
{
	if (auto _object = _obj_weak.lock(); _object)
	{
		
	}
}



void object::collision_update()
{
	_collision_component->setpos(vec(get_location()));
}


void object::release()
{

}

uint32_t object::get_layer_id() const&
{
	return layer_type::object;
}

void object::update()
{
	if (_owner && get_b_update_location_from_owner())
	{
		set_location(_owner->get_location() + get_distance_from_owner());
	}

	
}


