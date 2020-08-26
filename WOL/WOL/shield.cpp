#include "pch.h"
#include "shield.h"
#include "collision_mgr.h"

void shield::initialize()
{
	actor::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EShield, ECircle);

	auto sp_collision = _collision_component.lock();

	if (!sp_collision)return;

	sp_collision->_size = { 50.f,50.0f };

	duration = 5.f;
}

Event shield::update(float dt)
{
	actor::update(dt);

	duration -= dt;
	if (duration < 0)return Event::Die;

	Event _E = object::update(dt);

	auto Owner = _owner.lock();
	if (!Owner) return Event::Die;

	vec w = Owner->_transform->_location;

	vec& r = _transform->_dir;

	r = math::rotation_dir_to_add_angle(r, _speed* dt);

	_transform->_location = w + r * _shield_distance;

	return _E;
}


uint32_t shield::get_layer_id() const&
{
	return layer_type::EEffect;
};
