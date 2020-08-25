#include "pch.h"
#include "barrel.h"
#include "game.h"
#include "rotation_bullet.h"

void barrel::update()
{
	super::update();

	if (GetAsyncKeyState(VK_RIGHT))
	{
		set_angle(get_angle() + get_angle_speed());
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		set_angle(get_angle() - get_angle_speed());
	}

	using namespace std;

	const auto radian_angle = math::degree_to_radian(get_angle());

	super::set_direction(
		{ cosf(radian_angle),
		sinf(radian_angle) });
}

void barrel::render(HDC hdc)
{
	super::render(hdc);

	const auto [x,y] = get_location();
	const auto [dx, dy] = get_direction();

	MoveToEx(hdc, (int)x, (int)y, nullptr);

	LineTo(hdc,(int)x+ dx*_length,
			   (int)y+ dy* _length);
}

void barrel::initialize()
{
	super::initialize();

	super::set_render_type(render_type::none);
}

uint32_t barrel::get_layer_id() const&
{
	return super::get_layer_id()+1;
}

void barrel::fire()&
{
	const auto [x, y] = get_direction();

	auto _bullet = game::instance().insert_object<bullet>();

	auto [loc_x, loc_y] = get_location();

	loc_x += x * _length;
	loc_y += y * _length;

	_bullet->set_location(vec{ loc_x,loc_y });
	_bullet->set_direction(get_direction());
};


void barrel::rotation_fire()&
{
	const auto [x, y] = get_direction();

	auto _bullet = game::instance().insert_object<rotation_bullet>();

	auto [loc_x, loc_y] = get_location();

	loc_x += x * _length;
	loc_y += y * _length;

	_bullet->set_location(vec{ loc_x,loc_y });
	_bullet->set_direction(vec{ get_direction() });
}