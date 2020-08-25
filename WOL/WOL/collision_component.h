#pragma once
#include "collision_tag.h"

class object;

class collision_component
{
private:
	std::weak_ptr<class object> _owner{};
public:
	bool bCollision{ true };
	bool bRender{ true };

	void set_owner(std::weak_ptr<object> _owner)&;
	std::weak_ptr<object> get_owner()const { return _owner; }
	bool bPush{ true };
	void Hit(std::weak_ptr<class object> _target);
	figure_type _figure_type{ figure_type::ERect };

	vec _size;

	RECT make_rect();
	circle make_circle();
};

