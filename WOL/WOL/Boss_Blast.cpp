#include "pch.h"
#include "Boss_Blast.h"
#include "collision_mgr.h"
#include "Bmp_mgr.h"
#include "render_component.h"
#include "object_mgr.h"
#include "Effect.h"
#include "Freezing_Interface.h"
#include "sound_mgr.h"


void Boss_Blast::Hit(std::weak_ptr<object> _target)
{
	actor::Hit(_target);

	auto sp_Target = _target.lock();
	if (!sp_Target)return;
	
	// 속성 공격 확률 계산
	float DICE = math::Rand<float>({ 0.01f,1.f });
	if (DICE >= ICE_Percentage)return;

	if (sp_Target->ObjectTag == object::Tag::player)
	{
		auto sp_EnableFreezObject = std::dynamic_pointer_cast<Freezing_Interface>(sp_Target);
		if (!sp_EnableFreezObject)return;
		if (!_transform)return;

		vec MsgLocation = _transform->_location;
		
		sp_EnableFreezObject->_Freezing_Info.Freez(MsgLocation);
	}
}

void Boss_Blast::initialize()
{
	actor::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EMonsterAttack, ECircle);

	//RenderComponent Setup
	{
		PaintSizeX = 200;
		PaintSizeY = 250;
		ScaleY = ScaleX = 0.6f;
		constexpr COLORREF _ColorRef = COLOR::MRGENTA();
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
		sp_collision->bSlide = true;
		sp_collision->bCollision = true;
	};

	_Shadow.bShadow = false;
	_Shadow.correction = { 0,60 };

	ObjectTag = object::Tag::monster_attack;
	bAttacking = true;

	Duration = 0.4f;

	bAttacking = true;

	Attack = { 30,40 };

	sound_mgr::instance().RandSoundKeyPlay("ICE_BLAST", { 1,4 }, 1.f);

	UniqueID = EObjUniqueID::ICEBLAST;


	IceEffectPlay();

};

Event Boss_Blast::update(float dt)
{
	Event _Event = actor::update(dt);
	SoundTick -= dt;
	Duration -= dt;
	ICE_EffectTick -= dt;

	if (SoundTick < 0)
	{
		sound_mgr::instance().RandSoundKeyPlay("ICE_BLAST", { 1,4 }, 1.f);
		SoundTick = 1.f;
	}


	if (Duration < 0.3f)
	{
		_render_component->_Anim.EndMotionColIndex = 7;
	}
	if (Duration < 0)
	{
		IceEffectPlay();
		_Event = Event::Die;
	}

	
	return _Event;
};

void Boss_Blast::HitTile(RECT _rt)
{
	actor::HitTile(_rt);
	/*IceEffectPlay();
	bDie = true;*/
}

void Boss_Blast::render(HDC hdc, vec camera_pos, vec size_factor)
{
	actor::render(hdc, camera_pos, size_factor);

	if (ICE_EffectTick < 0)
	{
		IceEffectPlay();
		ICE_EffectTick = 0.9;
	}

}

void Boss_Blast::IceEffectPlay()
{
	if (!_transform)return;

	vec MyLocation = _transform->_location;
	collision_mgr::instance().HitEffectPush(MyLocation + math::RandVec() * math::Rand<int>({ -65,65 }), 0.1f);
}

uint32_t Boss_Blast::get_layer_id() const&
{
	return layer_type::EObject;
};



