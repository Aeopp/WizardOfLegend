#pragma once
#include "math.h"

class Transform
{
public:
	vec _location{};
	vec _dir{};
	vec _size{};

	// 
	// ������ �̸� �����ϰ� ȣ�����ּ���. ��ŸŸ���� ���ؼ� �Ѱ��ּ��� 
	void Move(float _speed);
};

