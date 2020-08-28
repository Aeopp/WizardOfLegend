#include "pch.h"
#include "transform.h"

void Transform::Move(float _speed)
{
	_location += _dir * _speed;
}
