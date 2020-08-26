#include "pch.h"
#include "object_mgr.h"
#include "object.h"
#include "player.h"
#include "Monster.h"
#include "Mouse.h"
#include "timer.h"
#include "Camera.h"

void object_mgr::render(HDC hdc)
{
	int count = 0;

	for (auto& [f, obj_list] : object_map)
	{
		for (auto& obj : obj_list)
		{
			count++; 
			obj->render(hdc, camera_pos);
		}
	}

	if (bDebug)
	{
		std::wstringstream wss;
		wss << L"오브젝트 개수 : " << count << std::endl; 
		RECT _rt{ 600,100,  900 , 200 }; 

		DrawText(hdc, wss.str().c_str(), wss.str().size(), &_rt, DT_CENTER);
	}
};

void object_mgr::update()
{
	using namespace std;

	float dt = Timer::instance().delta();

	for (auto& [f, obj_list] : object_map)
	{
		for (auto obj = begin(obj_list); obj != end(obj_list);)
		{
			Event _event = (*obj)->update(dt);

			switch (_event)
			{
			case None:
				++obj;
				break;
			case Die:
				obj = obj_list.erase(obj);
				break;
			default:
				break;
			}
		}
	}
};

void object_mgr::initialize()
{
	auto _ptr = insert_object<Player>();

	auto _camera = insert_object<Camera>();
	_camera->_owner = _ptr;

	this->_Camera = _camera;

	for (int i = 1; i < 100; ++i)
	{
		auto _ptr2 = insert_object<Monster>();
		_ptr2->_transform->_location = vec{ i *100,i*100};
	}
	
	auto EMouse = insert_object<Mouse>();
}

void object_mgr::release()
{
	object_map.clear();
}
