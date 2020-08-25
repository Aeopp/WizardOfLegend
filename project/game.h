#pragma once
#include "pch.h"
#include "singleton_interface.h"

class game : public singleton_interface<game>
{
public :
	void render(HDC hdc)override;
	void update()override;
	void initialize()override;
	void release()override;
public:
	std::map<uint32_t,std::list<std::shared_ptr<class object>>> object_list;

	template<typename object_type,typename ...param_Ty>
	[[nodiscard]] auto insert_object(param_Ty&&... _params);
};

template<typename object_type, typename ...param_Ty>
[[nodiscard]]  auto game::insert_object(param_Ty&& ..._params)
{
	std::shared_ptr<object_type> _ptr = std::make_shared<object_type>();

	_ptr->set_ptr(_ptr);

	_ptr->initialize();

	// if  constexpr (std::enable_if<decltype(std::declval<object_type>().late_initialize())>)
	//{
	//	_ptr->late_initialize(std::forward<param_Ty>(_params)...);
	//};

	object_list[_ptr->get_layer_id()].push_back(_ptr);

	return _ptr;
}
