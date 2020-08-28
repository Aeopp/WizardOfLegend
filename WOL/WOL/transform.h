#pragma once
#include "math.h"

class Transform
{
public:
	vec _location{};
	vec _dir{};
	vec _size{};

	// 
	// 방향을 미리 설정하고 호출해주세요. 델타타임을 곱해서 넘겨주세요 
	void Move(float _speed);
};

