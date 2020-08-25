#include "pch.h"
#include "rotation_bullet.h"
#include "game.h"


void rotation_bullet::render(HDC hdc)
{
	super::render(hdc);

	set_bullet_rotation_angle(get_bullet_rotation_angle() + get_angle_speed());
	const float _angle = get_bullet_rotation_angle();

	auto [x, y] = math::dir_from_angle(_angle);
	x *= _bullet_distance;
	y *= _bullet_distance;

	const auto [x1, y1] = get_location();

	x += x1;
	y += y1;

	const auto [sx, sy] = get_size();

	Ellipse(hdc, x - sx, y - sy, x + sx, y + sy);
}

void rotation_bullet::initialize()
{
	super::initialize();

	set_bullet_distance(30.0f);
	set_bullet_rotation_angle(0.0f);
	set_angle_speed(10.0f);
	set_speed(1.0f);
	set_size({ 10,10 });

	set_render_type(render_type::none);
}



