#pragma once
#include "moveable_object.h"

class shield : public moveable_object
{
private:
	using super = moveable_object;
public:
	virtual void initialize()override;
	virtual void update()override;
	virtual void render(HDC hdc)override;
};

