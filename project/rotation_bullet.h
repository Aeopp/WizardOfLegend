#pragma once
#include "bullet.h"

class rotation_bullet :
    public bullet
{
    using super = bullet;
public:
	virtual void render(HDC hdc)override;
	virtual void initialize()override;
private:
	GETSET(float,_bullet_distance)
	GETSET(float,_bullet_rotation_angle)
};

