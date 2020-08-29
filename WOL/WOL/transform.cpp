#include "pch.h"
#include "transform.h"

void Transform::Move(vec dir,float _speed)
{
	_location += dir * _speed;
}
