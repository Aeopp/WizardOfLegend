#include "pch.h"
#include "bullet.h"

void bullet::update()
{
	super::update();
	super::move();
}

void bullet::initialize()
{
	super::initialize();

	super::set_render_type(render_type::circle);
	super::set_size({ 25.0f,25.0f });
	super::set_speed(3.f);
}
