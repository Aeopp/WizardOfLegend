#include "pch.h"
#include "ICE_Crystal.h"
#include "collision_mgr.h"

void ICE_Crystal::initialize()
{
	actor::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EPlayerAttack, ERect);

	auto sp_collision = _collision_component.lock();

	if (!sp_collision)return;

	sp_collision ->_size = { 50.f,50.0f };
	sp_collision ->bRender = true;
	sp_collision ->bPush = false;
	sp_collision ->bCollision = true;

	Minimum_distance = 10.f;
	bSuccess = false;
	_target = vec{ 0.f,0.f };
	_speed = 300.f;
}

Event ICE_Crystal::update(float dt)
{
	Event _Event = actor::update(dt);

	vec& v = _transform->_location;

	if (!bSuccess)
	{
		vec w = _target;

		vec d = w - v;

		if (d.length() < Minimum_distance)
		{
			bSuccess = true;
			return Event::None;
		}

		vec dir = d.get_normalize();

		v += (dir * (_speed * dt));
	}
	else
	{
		auto Owner = _owner.lock();
		if (!Owner) return Event::Die;

		vec w = Owner->_transform->_location;

		vec d = w - v;

		if (d.length() < Minimum_distance)
		{
			return Event::Die;
		}

		vec dir = d.get_normalize();

		v += (dir * (_speed * dt));
	};

	return _Event;
}


uint32_t ICE_Crystal::get_layer_id() const&
{
	return layer_type::EEffect;
};

