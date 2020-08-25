#include "pch.h"
#include "shield.h"
#include "math.h"

void shield::initialize()
{
	super::initialize();

	super::set_size({ 33,33 });
	super::set_direction({ 1,0 });
	super::set_render_type(render_type::circle);
	super::set_angle_speed(10.f);
};

void shield::update()
{
	super::update();

	using namespace std;

	if (auto _owner = get_owner();_owner)
	{
		const auto _owner_location = _owner->get_location();

		const auto _owner_size = _owner->get_size();

const float _angle = get_angle();

const float _angle_radian = math::degree_to_radian(_angle);

		object::set_angle(_angle + 0.5f);

		set_location({
			_owner_location.x + cosf(_angle_radian) * _owner_size.x,
			_owner_location.y + sinf(_angle_radian) * _owner_size.y
			});
	}
}

void shield::render(HDC hdc)
{
	super::render(hdc);
}
