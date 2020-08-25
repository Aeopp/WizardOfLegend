#pragma once
#include "moveable_object.h"

class barrel : public  moveable_object
{
private:
	using super = moveable_object;
	float _length{ 100.0f };
public:
	virtual void update()override;
	virtual void render(HDC hdc)override;
	virtual void initialize()override;
	virtual uint32_t get_layer_id()const&override;
public:
	void fire()&;
	void rotation_fire()&;
};

