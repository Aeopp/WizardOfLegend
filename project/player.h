#pragma once
#include "moveable_object.h"

class player :
    public moveable_object
{
public:
	using super = moveable_object;

	virtual void update()override;
	virtual void render(HDC hdc)override;
	virtual void initialize()override;
	virtual void release()override;
public:
	void fire();
private:
	std::shared_ptr<class barrel> _barrel{ nullptr };
public:
	inline auto& get_barrel()const&
	{
		return _barrel;
	}
private:
};

