#include "pch.h"
#include "ICE_Blast.h"
#include "collision_mgr.h"
#include "Bmp_mgr.h"
#include "render_component.h"
#include "object_mgr.h"
#include "Effect.h"
#include "sound_mgr.h"

void ICE_Blast::initialize()
{
	actor::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EPlayerAttack, ECircle);

	//RenderComponent Setup
	{
		PaintSizeX = 200;
		PaintSizeY = 250;
		ScaleY = ScaleX = 0.6f;
		constexpr COLORREF _ColorRef = COLOR::MEGENTA();
		float AnimDuration = 0.7f;

		_render_component = std::make_shared<render_component>();
		_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(L"ICE_BLAST");
		_render_component->Default_Src_Paint_Size = vec{ PaintSizeX,PaintSizeY };
		_render_component->Dest_Paint_Size = vec{ PaintSizeX * ScaleX,PaintSizeY * ScaleY };
		_render_component->_ColorKey = _ColorRef;
		_render_component->_Img_src = RECT{ 0,0,PaintSizeX,PaintSizeY };
		_render_component->_Anim.SetAnimationClip(
			{ 3 }, AnimDuration);
		_render_component->_Anim.EndMotionColIndex = 2;
	};
	// 콜리전 셋업
	{
		auto sp_collision = _collision_component.lock();

		if (!sp_collision)return;
		sp_collision->bCollisionTargetPushFromForce = true;
		sp_collision->PushForce = 50.f;

		sp_collision->_size = { 30.f,30.0f };
		sp_collision->bRender = true;
		sp_collision->bSlide = false;
		sp_collision->bCollision = true;
	};

	_Shadow.bShadow = false;
	_Shadow.correction = { 0,60 };

	ObjectTag = object::Tag::player_attack;
	bAttacking = true;

	Duration = 0.8f;

	bAttacking = true;

	Attack = { 30,40 };

	sound_mgr::instance().RandSoundKeyPlay("ICE_BLAST", { 1,4 }, 1.f);

	UniqueID = EObjUniqueID::ICEBLAST;
};

Event ICE_Blast::update(float dt)
{
	Event _Event = actor::update(dt);
	Duration -= dt;
	if (Duration < 0.3f)
	{
		_render_component->_Anim.EndMotionColIndex = 7;
	}
	if (Duration < 0)
		_Event = Event::Die;

	return _Event;
};

void ICE_Blast::IceEffectPlay()
{
	
}

uint32_t ICE_Blast::get_layer_id() const&
{
	return layer_type::EObject;
};

