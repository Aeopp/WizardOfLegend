#include "pch.h"
#include "stalker.h"

void stalker::update()
{
	super::update();

	if (_target.expired())return;

	{
		auto target_ptr = _target.lock();

		auto dir  = (get_location() - target_ptr->get_location()).get_normalize();
		set_direction(dir);
		move();
	}
}

void stalker::initialize()
{
	super::initialize();

	set_speed(1.0f);
}
