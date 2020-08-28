#include "pch.h"
#include "object_mgr.h"
#include "object.h"
#include "player.h"
#include "Monster.h"
#include "Mouse.h"
#include "timer.h"
#include "Camera.h"

void object_mgr::render(HDC hdc,std::pair<float,float> size_factor)
{
	int count = 0;

	for (auto& [f, obj_list] : object_map)
	{
		for (auto& obj : obj_list)
		{
			count++; 
			obj->render(hdc, camera_pos, vec{ size_factor.first,size_factor.second });
		}
	}

	if (bDebug)
	{
		auto [sx, sy] = size_factor;

		std::wstringstream wss;
		wss << L"오브젝트 개수 : " << count << std::endl; 
		RECT _rt{ 1200 *sx,100*sy,  1400 *sx, 200*sy }; 

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

	check_erase();
};

void object_mgr::initialize()
{
	
}

void object_mgr::release()
{
	object_map.clear();
}

void object_mgr::check_erase()
{
	for (auto& [key, obj_list] : object_map)
	{
		obj_list.erase(

			std::remove_if(std::begin(obj_list), std::end(obj_list),
			[](auto& cur_obj) {if (!cur_obj)return true; return cur_obj->bDie; }),

			std::end(obj_list));
	};
}
