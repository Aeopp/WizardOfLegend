#pragma once
#include "Boomerang.h"

// �߰� ������ Ȱ�� �� ������
// ���ʸ� ���� �����ְ�
// �����̼� ���͸� �̸� ���� ���ִ� ������ �ϸ� �ɵ�
class RotationBoomerang :
	public Boomerang
{
public:
	void initialize()override;
	Event update(float dt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	float ImgAngle = 0;
	float RotationSpeed = 1600.f;
};

