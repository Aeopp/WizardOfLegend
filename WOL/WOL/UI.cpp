#include "pch.h"
#include "UI.h"

void UI::render(HDC hdc, vec camera_pos)
{
	vec loc  = _transform->_location;
	vec s =_transform->_size;

	Rectangle(hdc, loc.x - s.x, loc.y - s.y, loc.x + s.x, loc.y + s.y);
}
