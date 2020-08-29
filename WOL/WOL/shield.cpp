#include "pch.h"
#include "shield.h"
#include "collision_mgr.h"
#include "render_component.h"
#include "Color.h"
#include "Bmp_mgr.h"
#include "Bmp.h"





void shield::initialize()
{
	actor::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EShield, ECircle);

	auto sp_collision = _collision_component.lock();

	if (!sp_collision)return;

	sp_collision->_size = { 50.f,50.0f };

	duration = 15.f;
	HoleDuration = 5.f;
	PaintSizeX = 100;
	PaintSizeY = 105;
	float Scale = 1.f;
	_speed = 120.f;

	_render_component = std::make_shared<render_component>();
	_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(L"GAIA_ARMOR");
	_render_component->Default_Src_Paint_Size = vec{ PaintSizeX,PaintSizeY };
	_render_component->Dest_Paint_Size = vec{ PaintSizeX * Scale,PaintSizeY * Scale };
	_render_component->_ColorKey = COLOR::MEGENTA();
	_render_component->_Img_src = RECT{ 0,0,PaintSizeX,PaintSizeY };
	_render_component->_Anim.SetAnimationClip(
		{ 12}, 360.f/_speed);



}

Event shield::update(float dt)
{
	actor::update(dt);

	duration -= dt;
	if (duration < 0)return Event::Die;

	HoleDuration -= dt;
	if (HoleDuration < 0)bHoleRender = false;


	DegreeTick -= dt;
	if (DegreeTick < 0) {
		DegreeTick = DegreePerSecond;
		++ImgIndex;
		ImgIndex %= 12;
	}

	Event _E = object::update(dt);

	auto Owner = _owner.lock();
	if (!Owner) return Event::Die;

	vec w = Owner->_transform->_location;

	vec& r = _transform->_dir;

	r = math::rotation_dir_to_add_angle(r, _speed* dt);

	_transform->_location = w + r * _shield_distance;

	return _E;
}


void shield::render(HDC hdc, vec camera_pos, vec size_factor)
{

	if (bHoleRender)
	{
		static auto sp_Hole = Bmp_mgr::instance().Find_Image_SP(L"BOTTOM_HOLE");
		if (sp_Hole) 
		{
			vec v = HoleLocation - camera_pos;
			GdiTransparentBlt(hdc, v.x, v.y, 200, 150, sp_Hole->Get_MemDC(),
				0, 0, 200, 150, COLOR::MEGENTA());
		}
	}

	actor::render(hdc, camera_pos, size_factor);
}

void shield::CalcIdx()
{
	Angle = math::Angle360conversion(Angle);

	int Idx = (Angle / 30) - 9;
	if (Idx < 0) Idx += 12;

	_render_component->_Anim.ColIndex = Idx;
}

uint32_t shield::get_layer_id() const&
{
	return layer_type::EEffect;
};
