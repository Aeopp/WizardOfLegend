#include "pch.h"
#include "Boomerang.h"
#include "collision_mgr.h"
#include "render_component.h"
#include "Color.h"
#include "Bmp_mgr.h"

void Boomerang::initialize()
{
	actor::initialize();

	_collision_component_lower = collision_mgr::instance().
	insert(_ptr, collision_tag::EPlayerAttack, ECircle);

	auto sp_collision = _collision_component_lower.lock();

	if (!sp_collision)return;

	sp_collision->_size = { 25.f,25.0f };

	Duration = 30.f;

	PaintSizeX = 200;
	PaintSizeY = 200;
	float Scale = 0.8f;
	// 자기자신의 회전속도임
	_speed = 360.f;
	RotationSpeedDegree = 240.f;
	DistancefromCenter = 100.f;

	_render_component = std::make_shared<render_component>();
	_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(L"SWORDMAN_ATTACK");
	_render_component->Default_Src_Paint_Size = vec{ PaintSizeX,PaintSizeY };
	_render_component->Dest_Paint_Size = vec{ PaintSizeX * Scale,PaintSizeY * Scale };
	_render_component->_ColorKey = RGB(255, 0, 255);
	_render_component->_Img_src = RECT{ 0,0,PaintSizeX,PaintSizeY };
	_render_component->_Anim.SetAnimationClip(
		{ 4,4,4,4,4,4 }, 360.f / _speed);

	TickScale = 1.f;
	sp_collision->bSlide = false;
	id = object::ID::player_attack;
	bAttacking = true;

	_Shadow.bShadow = false;
	//_Shadow.correction = { 0,120 };
}

Event Boomerang::update(float dt)
{
	Event _event = actor::update(dt);

	Temp += dt;
	if (Temp > 0.2)
	{
		Temp = 0;
		_render_component->_Anim.RowIndex++;
		_render_component->_Anim.RowIndex %= 6;
	}

	Duration -= dt;
	if (Duration < 0)return Event::Die;

	Tick += dt;
	float c = (cosf(Tick) * TickScale) + TickScale;

	float _RotationSpeedDegree = RotationSpeedDegree * c;
	float  _DistancefromCenter = DistancefromCenter * c;
	float Animspeed = (360.f / _speed * c);

	if (!_render_component)return Event::Die;
	_render_component->_Anim.AnimDuration = Animspeed;

	auto Owner = _owner.lock();
	if (!Owner) return Event::Die;

	vec w = Owner->_transform->_location;
	vec& r = _transform->_dir;
	r = math::rotation_dir_to_add_angle(r, _RotationSpeedDegree * dt);

	_transform->_location = w + r * _DistancefromCenter;

	return _event;
}

uint32_t Boomerang::get_layer_id() const&
{

	return layer_type::EEffect;
}
