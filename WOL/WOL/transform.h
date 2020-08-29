#pragma once
#include "math.h"

class Transform
{
public:
	vec _location{};
	vec _dir{};
	vec _size{};

	// 
	void Move(vec dir ,float _speed);
};

