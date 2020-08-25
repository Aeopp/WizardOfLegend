#include "pch.h"
#include "player.h"
#include "collision_mgr.h"
#include "Input_mgr.h"
#include "timer.h"


void Player::initialize()
{
	object::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::player, ECircle);
	if (!_collision_component)return;

	_collision_component->_size = { 50.f,50.0f };
};

void Player::update(float dt)
{
	object::update(dt);

	if (Input_mgr::instance().Key_Pressing(VK_RIGHT))
	{
		if(Input_mgr::instance().Key_Pressing(VK_UP))
		{
			_transform->_location += vec{ +1,0 }*_speed*sqrtf(2.f) * dt;
			_transform->_location += vec{ 0,-1 }*_speed * sqrtf(2.f) * dt;
		}
		if (Input_mgr::instance().Key_Pressing(VK_DOWN))
		{
			_transform->_location += vec{ +1,0 }*_speed * sqrtf(2.f) * dt;
			_transform->_location += vec{ 0,+1 }*_speed * sqrtf(2.f) * dt;
		}
		else 
			_transform->_location += vec{ +1,0 }*_speed * dt;
	}
	if (Input_mgr::instance().Key_Pressing(VK_LEFT))
	{
		if (Input_mgr::instance().Key_Pressing(VK_UP))
		{
			_transform->_location += vec{ -1,0 }*_speed * sqrtf(2.f) * dt;
			_transform->_location += vec{ 0,-1 }*_speed * sqrtf(2.f) * dt;
		}
		if (Input_mgr::instance().Key_Pressing(VK_DOWN))
		{
			_transform->_location += vec{ -1,0 }*_speed * sqrtf(2.f) * dt;
			_transform->_location += vec{ 0,+1 }*_speed * sqrtf(2.f) * dt;
		}
		else
			_transform->_location += vec{ -1,0 }*_speed * dt;
	}

	 if (Input_mgr::instance().Key_Pressing(VK_UP))
	{
		_transform->_location += vec{ 0,-1 }*_speed * dt;;
	}
	 if (Input_mgr::instance().Key_Pressing(VK_DOWN))
	{
		_transform->_location += vec{ 0,+1 }*_speed * dt; ;
	}
}

void Player::render(HDC hdc,vec camera_pos)
{
	object::render(hdc, camera_pos);

}
