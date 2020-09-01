#include "pch.h"
#include "object_mgr.h"
#include "object.h"
#include "player.h"
#include "Monster.h"
#include "Mouse.h"
#include "timer.h"
#include "Camera.h"
#include "game.h"
#include "Font.h"

void object_mgr::render(HDC hdc,std::pair<float,float> size_factor)
{
	std::vector<std::shared_ptr<class object>> RenderSortY;

	RenderSortY.reserve(object_map[layer_type::EObject].size());

	std::copy(std::begin(object_map[layer_type::EObject]),
		std::end(object_map[layer_type::EObject]),
		std::back_inserter(RenderSortY));

	std::sort(std::begin(RenderSortY), std::end(RenderSortY),
		[](std::shared_ptr<object>& lhs, std::shared_ptr<object>& rhs) {
			if (!lhs->_transform) return false;
			if (!rhs->_transform) return false;

			return lhs->_transform->_location.y < rhs->_transform->_location.y;
		});
	for (auto& obj : RenderSortY)
	{
		vec culling_obj_pos = obj->_transform->_location - camera_pos;
		if (math::RectInPoint(game::client_rect, culling_obj_pos))
		{
			obj->render(hdc, camera_pos, vec{ size_factor.first,size_factor.second });
		}
	}
	RenderSortY.clear();

	/*for (auto& [object_layer, obj_list] : object_map)
	{
		if (object_layer == layer_type::EObject)continue;

		for (auto& obj : obj_list)
		{
			if (!obj->_transform)continue;

			if(object_layer >= layer_type::EEffect)
			{
				count++;
				obj->render(hdc, camera_pos, vec{ size_factor.first,size_factor.second });
			}
		}
	}*/

	for (auto& [_Color,TEffects ]: TextEffectMap)
	{
		for (auto TEffect= std::begin(TEffects);TEffect!= std::end(TEffects);)
		{
			vec v = TEffect->pos - camera_pos;

			Font FontOn = Font(hdc, _Color, v.x, v.y,
				TEffect->size, TEffect->Text, TEffect->Text, TEffect->Text, TEffect->Text);

			TEffect->pos -= TEffect->dir;
			TEffect->duration -= DeltaTime;

			if (TEffect->duration < 0)
			{
				TEffect = TEffects.erase(TEffect);
			}
			else
			{
				++TEffect;
			}
		}
	}


	/*if (bDebug)
	{
		auto [sx, sy] = size_factor;

		std::wstringstream wss;
		wss << L"오브젝트 개수 : " << count << std::endl; 
		RECT _rt{ 1200 *sx,100*sy,  1400 *sx, 200*sy }; 

		DrawText(hdc, wss.str().c_str(), wss.str().size(), &_rt, DT_LEFT);
	}*/
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

void object_mgr::UIEffectRender(HDC hdc, vec camera_pos, std::pair<float, float> size_factor)
{
	for (int i = layer_type::EEffect; i <= layer_type::ENone; ++i)
	{
		for (auto& obj : object_map[i])
		{
			if (!obj->_transform)continue;
			obj->render(hdc, camera_pos, vec{ size_factor.first,size_factor.second });
		}
	}
}

void object_mgr::check_erase()
{
	for (auto& [key, obj_list] : object_map)
	{
		obj_list.erase(

			std::remove_if(std::begin(obj_list), std::end(obj_list),
			[](auto& cur_obj) {if (!cur_obj)return true; 
				return cur_obj->bDie; }),

			std::end(obj_list));
	};
}
