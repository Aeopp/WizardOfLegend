#include "pch.h"
#include "actor.h"
#include "render_component.h"

void actor::initialize()
{
	object::initialize();

	_Shadow.initialize();

	auto sp_ptr = _ptr.lock();
	if (!sp_ptr)return;

	_Shadow._owner = std::dynamic_pointer_cast<actor>(sp_ptr);
}

void actor::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!_render_component) return;
	_render_component->Dest_Loc = _transform->_location - camera_pos - (_render_component->Dest_Paint_Size*0.5);

	_Shadow.render(hdc, camera_pos);
	_render_component->Render(hdc);
}
