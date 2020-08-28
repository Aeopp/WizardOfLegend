#pragma once
#include "singleton_interface.h"

class object_mgr : public singleton_interface<object_mgr>
{

public:
	void render(HDC hdc,std::pair<float,float> size_factor);
	void update();
	void initialize() ;
	void release();

	vec camera_pos{};

	std::weak_ptr<class Camera> _Camera;
private:
	std::map<uint32_t,std::list<std::shared_ptr<class object>>> object_map;
public:
	template<typename object_type, typename ...param_Ty>
	[[nodiscard]] auto insert_object(param_Ty&&... _params);
private:
	void check_erase();
};

template<typename object_type, typename ...param_Ty>
[[nodiscard]] auto object_mgr::insert_object(param_Ty&& ..._params)
{
	std::shared_ptr<object_type> _ptr = std::make_shared<object_type>();

	_ptr->set_ptr(_ptr);

	_ptr->initialize();
	
	if  constexpr (std::is_member_function_pointer<decltype(&object_type::late_initialize)>::value)
	{
		_ptr->late_initialize(std::forward<param_Ty>(_params)...);
	};

	object_map[_ptr->get_layer_id()].push_back(_ptr);

	return _ptr;
}


