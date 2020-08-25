#include "pch.h"
#include "object_mgr.h"
#include "object.h"

void object_mgr::render(HDC hdc)
{
	for (auto& [f,obj_list] : object_map)
	{
		for (auto& obj : obj_list)
		{
			obj->render(hdc);
		}
	}
}

void object_mgr::update()
{
	for (auto& [f, obj_list] : object_map)
	{
		for (auto& obj : obj_list)
		{
			obj->update();
		}
	}
}

void object_mgr::initialize()
{

}

void object_mgr::release()
{
	object_map.clear();
}