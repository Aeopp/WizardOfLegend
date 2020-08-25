#include "pch.h"
#include "moveable_object.h"

void moveable_object::move()&
{
	auto current_location = get_location();

	object::set_location(
		{ current_location.x + direction.x * _speed, current_location.y + direction.y * _speed });
};

void moveable_object::rotation(const float degree)&
{
	set_direction(math::rotation_dir_to_add_angle(get_direction(), degree));
}

void moveable_object::rotation_from_owner(const float degree)&
{
	if (auto _owner = get_owner(); _owner)
	{
		 auto dir=	get_location();
		 const auto _owner_location = get_owner()->get_location();

		dir.x -= _owner_location.x;
		 dir.y -= _owner_location.y;

		const auto [x, y] = math::rotation_dir_to_add_angle( dir,degree);
		
		set_location({ _owner_location.x + x * _owner_distance,
			_owner_location.y + y * _owner_distance});
	}
}
