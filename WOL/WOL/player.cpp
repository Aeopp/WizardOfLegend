#include "pch.h"
#include "player.h"
#include "collision_mgr.h"
#include "Input_mgr.h"
#include "timer.h"
#include "Camera.h"
#include "game.h"
#include "shield.h"
#include "object_mgr.h"
#include "Mouse.h"
#include "ICE_Crystal.h"
#include "timer.h"


void Player::initialize()
{
	object::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EPlayer, ECircle);
	if (!_collision_component)return;

	_collision_component->_size = { 50.f,50.0f };

	_Camera = object_mgr::instance()._Camera;
};

Event Player::update(float dt)
{
	Event _E = object::update(dt);

	player_check(dt);

	MakeShield();

	return _E;
}

void Player::render(HDC hdc, vec camera_pos)
{
	object::render(hdc, camera_pos);
};

void Player::temp(float temp)
{
	MessageBox(game::instance().hWnd, __FUNCTIONW__, __FUNCTIONW__, MB_OK);
}

void Player::MakeShield()
{
	Input_mgr& _Input = Input_mgr::instance();

	if (!_Input.Key_Down('Q'))return;

	static float shield_distance = 150.0f;
	static float shield_speed = 360.f;

	object_mgr& _obj_mgr = object_mgr::instance();

	float degree=360.0f / 8.f;

	for (int i = 0; i < 8; ++i)
	{
		auto _shield = _obj_mgr.insert_object<shield>();

	   _shield->_transform->_dir = math::dir_from_angle(degree * i);

		if (!_shield) return;
		_shield->_owner = _ptr;

		_shield->_transform->_location = _transform->_location + _shield->_transform->_dir * shield_distance;

		_shield->_shield_distance = shield_distance;
		_shield->_speed = shield_speed;
	}
}

void Player::ICE_CRYSTAL()
{
	Input_mgr& _Input = Input_mgr::instance();

	auto V= _Input.GetWorldMousePos();

	if (!V)return;

	auto _ICE = object_mgr::instance().insert_object<ICE_Crystal>();

	if (!_ICE) return;


	_ICE->_owner = _ptr;
	_ICE->_target = *V;
	_ICE->_transform->_location = _transform->_location;

}
void Player::Camera_Shake(float force,vec dir,float duration)
{
	auto Cam = _Camera.lock();

	if (!Cam)
	{
		this->_Camera = object_mgr::instance()._Camera;
		return;
	};

	Cam->camera_shake(force, dir ,duration);
};

void Player::player_check(float dt)
{
	Input_mgr& _Input = Input_mgr::instance();

	if (_Input.Key_Pressing(VK_RIGHT))
	{
		if (_Input.Key_Pressing(VK_UP))
		{
			float f = 1/ sqrtf(2.f);

			_transform->_location += vec{ +1,0 }*_speed * f * dt;
			_transform->_location += vec{ 0,-1 }*_speed * f * dt;
		}
		if (_Input.Key_Pressing(VK_DOWN))
		{
			float f = 1 / sqrtf(2.f);

			_transform->_location += vec{ +1,0 }*_speed * f * dt;
			_transform->_location += vec{ 0,+1 }*_speed * f* dt;
		}
		else
			_transform->_location += vec{ +1,0 }*_speed * dt;
	}
	if (_Input.Key_Pressing(VK_LEFT))
	{
		if (_Input.Key_Pressing(VK_UP))
		{
			float f = 1 / sqrtf(2.f);

			_transform->_location += vec{ -1,0 }*_speed * f* dt;
			_transform->_location += vec{ 0,-1 }*_speed * f* dt;
		}
		if (_Input.Key_Pressing(VK_DOWN))
		{
			float f = 1 / sqrtf(2.f);

			_transform->_location += vec{ -1,0 }*_speed * f* dt;
			_transform->_location += vec{ 0,+1 }*_speed *f* dt;
		}
		else
			_transform->_location += vec{ -1,0 }*_speed * dt;
	}

	if (_Input.Key_Pressing(VK_UP))
	{
		_transform->_location += vec{ 0,-1 }*_speed * dt;;
	}
	if (_Input.Key_Pressing(VK_DOWN))
	{
		_transform->_location += vec{ 0,+1 }*_speed * dt; ;
	}

	if (_Input.Key_Down('W'))
	{
		ICE_CRYSTAL();
	}

	if (_Input.Key_Down(VK_LBUTTON))
	{
		Camera_Shake(5, vec{ 1,1 }, 0.1f);
	}
}
