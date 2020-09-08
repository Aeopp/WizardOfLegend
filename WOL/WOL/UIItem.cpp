#include "pch.h"
#include "UIItem.h"
#include "render_component.h"
#include "game.h"
#include "Color.h"
#include "Bmp_mgr.h"
#include "Input_mgr.h"
#include "sound_mgr.h"
#include "object_mgr.h"

void UIItem::initialize()
{
	UIInteraction::initialize();

	_render_component = std::make_shared<render_component>();
	_render_component->Default_Src_Paint_Size = vec{ 106,155};
	_render_component->_ColorKey = COLOR::MRGENTA();
	_render_component->_Img_src = RECT{ 0,0,106,155};
	_render_component->_RenderDesc = ERender::Transparent;

	_transform->_location = vec{ 0,0 };
	Scale = 1.f; 
	_transform->_size = vec{ 106* Scale,155* Scale };
}

void UIItem::render(HDC hdc, vec camera_pos, vec size_factor)
{
	object::render(hdc, camera_pos, size_factor);

	vec loc = _transform->_location;
	vec s = _transform->_size;
	vec ps = _render_component->Dest_Paint_Size;
	vec ds = _render_component->Default_Src_Paint_Size;

	s.x *= size_factor.x;
	s.y *= size_factor.y;

	if (!_render_component)return;

	ps.x *= size_factor.x;
	ps.y *= size_factor.y;

	loc.y *= size_factor.y;
	loc.x *= size_factor.x;

	loc -= _render_component->Dest_Paint_Size * Scale * 0.5f;
	loc -= camera_pos;

	_render_component->Dest_Loc = loc;
	_render_component->Dest_Paint_Size.x = ds.x * Scale * size_factor.x;
	_render_component->Dest_Paint_Size.y = ds.y * Scale * size_factor.y;

	_render_component->Render(hdc);
}

void UIItem::OnMouse(const vec hit_pos)
{




	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		Click(*Input_mgr::instance().GetWindowMousePos());
	}
}

void UIItem::Click(const vec hit_pos)
{

	int i = 0;


	if (_Event)
		if (_Event())
		{
			SOUNDPLAY("GET_SKILL", 1.f, false);
				bDie = true;
		}
			
}

Event UIItem::update(float dt)
{
	UI::update(dt);
	SoundTick -= dt;

	Input_mgr& _Input_mgr = Input_mgr::instance();
	auto pos = _Input_mgr.GetWindowMousePos();
	Input_mgr::instance()._Mouse;

	if (!pos) return Event::None;

	vec v = _transform->_location  - object_mgr::instance().camera_pos; 
	vec s = _transform->_size;

	RECT rt = make_rect(v.x, v.y, v.x + s.x, v.y + s.y);

	bool bClick = _Input_mgr.Key_Down(VK_LBUTTON);

	if (PtInRect(&rt, make_pt(pos->x, pos->y)))
	{
		bOnMouse = true;
		if (bClick)
		{
			if (SoundTick < 0)
			{ 
				SoundTick = 0.2f;
			}

			Click(*pos);
		}
		else
		{
			OnMouse(*pos);
		}
	}
	else
	{
		bOnMouse = false;
	}

	return Event::None;
}

uint32_t UIItem::get_layer_id() const&
{
	return layer_type::EUIIcon;
}

void UIItem::SetUp(vec location, std::wstring ImgKey,vec PaintSize,std::function<bool()> Event)
{
	if (!_transform)return;
	if (!_render_component)return;

	_transform->_location = std::move(location);
	_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(ImgKey);

	_transform->_size = vec{ PaintSize.x,PaintSize.y };
	_render_component->_Img_src = RECT{ 0,0,(LONG)PaintSize.x,(LONG)PaintSize .y};
	_render_component->Default_Src_Paint_Size = vec{ PaintSize.x,PaintSize.y };
	_render_component->Dest_Paint_Size= vec{ PaintSize.x,PaintSize.y };
	_render_component->Default_Src_Paint_Size = vec{ PaintSize.x,PaintSize.y };
	_Event = std::move(Event);

}
