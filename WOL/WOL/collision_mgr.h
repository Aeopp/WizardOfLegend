#pragma once
#include "pch.h"
#include "singleton_interface.h"
#include "collision_component.h"

class collision_mgr : public singleton_interface<collision_mgr>
{
public:
	std::map<collision_tag, std::list<std::shared_ptr<collision_component>>> _collision_map;

	std::weak_ptr< collision_component> insert(std::weak_ptr<class object> _owner,collision_tag _tag,
		figure_type _type = ERect);

	bool bRender = true;

	void collision(collision_tag lhs, collision_tag rhs);
	void render(HDC hdc, std::pair<float, float>  size_factor) ;
	void update();
	void release();
private:
	void check_erase()&;
};

