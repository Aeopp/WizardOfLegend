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
#include "helper.h"

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
	size_t RenderObjCount = 0;

	RECT CullingRect = game::client_rect;

	int width = (game::client_rect.right - game::client_rect.left )/ 2;
	int height = (game::client_rect.bottom - game::client_rect.top) / 2;
	CullingRect.left -= width;
	CullingRect.right += width;
	CullingRect.top -= height;
	CullingRect.bottom += height;

	for (auto& Deco: object_map[layer_type::EMapDeco])
	{
		if (!Deco->_transform)continue;

		vec culling_obj_pos = Deco->_transform->_location - camera_pos;
		if (math::RectInPoint(CullingRect, culling_obj_pos))
		{
			RenderObjCount++;

			Deco->render(hdc, camera_pos, vec{ size_factor.first,size_factor.second });
		}
	}


	for (auto& obj : RenderSortY)
	{
		if (!obj->_transform)continue;

		vec culling_obj_pos = obj->_transform->_location - camera_pos;
		if (math::RectInPoint(CullingRect, culling_obj_pos))
		{
			RenderObjCount++;

			obj->render(hdc, camera_pos, vec{ size_factor.first,size_factor.second });
		}
	}
	RenderSortY.clear();

	for (auto& [_Color,TEffects ]: TextEffectMap)
	{
		for (auto TEffect= std::begin(TEffects);TEffect!= std::end(TEffects);)
		{
			vec v = TEffect->pos - camera_pos;

			Font FontOn = Font(hdc, _Color, v.x, v.y,
				TEffect->size, TEffect->Text/*, TEffect->Text, TEffect->Text, TEffect->Text*/
			);

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


	if (bDebug)
	{
		auto [sx, sy] = size_factor;

		std::wstringstream wss;
		wss << L"드로우 오브젝트 개수 : " << RenderObjCount << std::endl;
		//RECT _rt{ 900 *sx,100*sy,  1200*sx, 200*sy }; 
		Font(hdc, RGB(111, 111, 111), 900, 200, 30, wss.str());
		//DrawText(hdc, wss.str().c_str(), wss.str().size(), &_rt, DT_LEFT);
		int count = 0;
		for (auto& [Tag, Object_List] : object_map)
		{
			count+=Object_List.size();
		}
		helper::TEXTOUT(hdc, 900, 100, L"오브젝트 개수 : ", count);
	}
};

void object_mgr::update()
{
	using namespace std;

	float dt = Timer::instance().delta();

	RECT ObjUpdateCullingRange = game::client_rect;

	ObjUpdateCullingRange.left -= ObjectUpdateRangeX;
	ObjUpdateCullingRange.top -= ObjectUpdateRangeY;
	ObjUpdateCullingRange.right += ObjectUpdateRangeX; 
	ObjUpdateCullingRange.bottom += ObjectUpdateRangeY;

	for (auto& [f, obj_list] : object_map)
	{
		for (auto obj = begin(obj_list); obj != end(obj_list);)
		{
			auto sp_Transform = (*obj)->_transform;
			if (!sp_Transform)continue;
			vec obj_location = sp_Transform->_location;

			//if (!math::RectInPoint(ObjUpdateCullingRange, obj_location ))continue;

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
	RECT rt = game::client_rect;

	ObjectUpdateRangeX = rt.right - rt.left;
	ObjectUpdateRangeY = rt.bottom - rt.top;
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
