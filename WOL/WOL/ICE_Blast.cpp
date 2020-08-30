#include "pch.h"
#include "ICE_Blast.h"
#include "collision_mgr.h"
#include "Bmp_mgr.h"
#include "render_component.h"
#include "object_mgr.h"
#include "Effect.h"

void ICE_Blast::initialize()
{
	actor::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EPlayerAttack, ECircle);

	//RenderComponent Setup
	{
		PaintSizeX = 200;
		PaintSizeY = 250;
		ScaleY = ScaleX = 0.7f;
		COLORREF _ColorRef = COLOR::MEGENTA();
		float AnimDuration = 0.5f;

		_render_component = std::make_shared<render_component>();
		_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(L"ICE_BLAST");
		_render_component->Default_Src_Paint_Size = vec{ PaintSizeX,PaintSizeY };
		_render_component->Dest_Paint_Size = vec{ PaintSizeX * ScaleX,PaintSizeY * ScaleY };
		_render_component->_ColorKey = _ColorRef;
		_render_component->_Img_src = RECT{ 0,0,PaintSizeX,PaintSizeY };
		_render_component->_Anim.SetAnimationClip(
			{ 8 }, AnimDuration);


		// 해당 스킬만을 위한 세팅....
		{
			_render_component->_Anim.ColIndex = 2;
			_render_component->_Anim.CurDelta = FLT_MAX;
		}
	};
	// 콜리전 셋업
	{
		auto sp_collision = _collision_component.lock();

		if (!sp_collision)return;
		sp_collision->bPush = true;
		sp_collision->PushForce = 20.f;

		sp_collision->_size = { 30.f,30.0f };
		sp_collision->bRender = true;
		sp_collision->bSlide = false;
		sp_collision->bCollision = true;
	};

	Minimum_distance = 10.f;
	bSuccess = false;
	_target = vec{ 0.f,0.f };
	_speed = 500.f;

	_Shadow.correction = { 0,60 };
}



Event ICE_Blast::update(float dt)
{
	Event _Event = actor::update(dt);

	vec& v = _transform->_location;

	if (!bSuccess)
	{
		vec w = _target;

		vec d = w - v;

		if (d.length() < Minimum_distance)
		{
			bSuccess = true;
			return Event::None;
		}

		vec dir = d.get_normalize();

		v += (dir * (_speed * dt));
	}
	else
	{
		auto Owner = _owner.lock();
		if (!Owner) return Event::Die;

		vec w = Owner->_transform->_location;

		vec d = w - v;

		if (d.length() < Minimum_distance)
		{
			auto ICE_EFFECT = object_mgr::instance().insert_object<Effect>(v.x, v.y,
				L"ICE_BLAST", layer_type::EEffect, 8, 0, 0.3f, 0.3f, 200, 250, ScaleX, ScaleY);
			ICE_EFFECT->CurrentCol = 3;

			return Event::Die;
		}

		vec dir = d.get_normalize();

		v += (dir * (_speed * dt));
	};

	return _Event;
}


uint32_t ICE_Blast::get_layer_id() const&
{
	return layer_type::EEffect;
};

