#pragma once
#include "math.h"

class Transform
{
public:
	vec _location{0,0};
	vec _dir{0,1};
	vec _size{1,1};

	// 
	void Move(vec dir ,float _speed);
};

