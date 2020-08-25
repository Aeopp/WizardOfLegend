#include "pch.h"
#include "player.h"
#include "collision_mgr.h"
#include "Input_mgr.h"

void Player::initialize()
{
	object::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::player, ERect);
	if (!_collision_component)return;

	_collision_component->_size = { 50.f,50.0f };
};


void Player::update()
{
	object::update();

	Input_mgr::instance().Event_Regist(VK_RIGHT, key_event::EHold, [this]() {
		_transform->_location += {1* 0.0002f };});

	Input_mgr::instance().Event_Regist(VK_LEFT, key_event::EHold, [this]() {
		_transform->_location += {-1* 0.0002f, 0};	});

	Input_mgr::instance().Event_Regist(VK_UP, key_event::EHold, [this]() {
		_transform->_location += {0,-1* 0.0002f}; });

	Input_mgr::instance().Event_Regist(VK_DOWN, key_event::EHold, [this]() {
		_transform->_location += {0, +1 * 0.0002f};	});

}

void Player::render(HDC hdc)
{
	object::render(hdc);

}
