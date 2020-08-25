#pragma once
#include "pch.h"
#include <type_traits>


enum figure_type : uint8_t
{
	ERect,
	ECircle,
};
class collision_component
{
public:
	GETSET(vec, pos);
	GETSET(vec, size);
	GETSET(figure_type, _tag);
	GETSET(std::weak_ptr<class object>, _owner);
	bool bPush{ true};

	void render(HDC hdc);

	circle get_circle()const&
	{
		return circle{ pos,size.x };
	}
	RECT get_rect()const&
	{
		RECT _rt;
		_rt.left = pos.x - size.x;
		_rt.top = pos.y - size.y;
		_rt.right = pos.x + size.x;
		_rt.bottom = pos.y + size.y;
		return _rt;
	};
};

 void collision_component::render(HDC hdc)
{
	 const float w = size.x;
	 const float h = size.y;
	 const float x = pos.x;
	 const float y = pos.y;

	 switch (_tag)
	 {
	 case ERect:
		 Rectangle(hdc, x - w, y - h, x + w, y + h);
		 break;
	 case ECircle:
		 Ellipse(hdc, x - w, y - h, x + w, y + h);
		 break;
	 default:
		 break;
	 };

}
