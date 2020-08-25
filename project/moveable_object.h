#pragma once
#include "object.h"

class moveable_object :
    public object
{
private:
    vec direction{};
    float _speed{ 1.0f };
    GETSET(float,_owner_distance)
public:
    inline  auto get_direction()const&
    {
        return direction;
    }
    inline void set_direction(decltype(direction) _direction)&
    {
        direction = _direction;
    }
	inline void set_speed(decltype(_speed) _speed)&
	{
        this->_speed = _speed;
	}
	inline  auto get_speed()const&
	{
		return _speed;
	}
public:
    void move()&;
    void rotation(const float degree)&;
    void rotation_from_owner(const float degree)&;
};

