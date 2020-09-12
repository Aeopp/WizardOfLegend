#include "pch.h"
#include "ICE_Blast.h"
#include "collision_mgr.h"
#include "Bmp_mgr.h"
#include "render_component.h"
#include "object_mgr.h"
#include "Effect.h"
#include "sound_mgr.h"
#include "Monster.h"
#include "Bmp.h"

void ICE_Blast::initialize()
{
	actor::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EPlayerAttack, ECircle);

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

	ObjectTag = object::Tag::player_attack;
	bAttacking = true;

	Duration = 0.8f;

	bAttacking = true;

	Attack = { 30,40 };

	sound_mgr::instance().RandSoundKeyPlay("ICE_BLAST", { 1,4 }, 1.f);

	UniqueID = EObjUniqueID::ICEBLAST;

	
	IceEffectPlay();
};

Event ICE_Blast::update(float dt)
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

void ICE_Blast::Hit(std::weak_ptr<object> _target)
{
	actor::Hit(_target);

	auto sp_Target = _target.lock();
	if (!sp_Target)return;

	// 속성 공격 확률 계산
	float DICE = math::Rand<float>({ 0.01f,1.f });
	if(DICE>=ICE_Percentage)return ;
	
	if ( sp_Target->ObjectTag == object::Tag::monster  )
	{
		auto sp_EnableFreezObject = std::dynamic_pointer_cast<Freezing_Interface>(sp_Target);
		if (!sp_EnableFreezObject)return;
		if (!_transform)return;
		
		vec MsgLocation = _transform->_location;
		
		sp_EnableFreezObject->_Freezing_Info.Freez(MsgLocation);
	}
}

void ICE_Blast::HitTile(RECT _rt)
{
	actor::HitTile(_rt);
	IceEffectPlay();
	bDie = true;
}

void ICE_Blast::render(HDC hdc, vec camera_pos, vec size_factor)
{
	actor::render(hdc, camera_pos, size_factor);

	if (ICE_EffectTick < 0)
	{
		IceEffectPlay();
		ICE_EffectTick = 0.9;
	}
	
}

void ICE_Blast::IceEffectPlay()
{
	if (!_transform)return;
	

	vec MyLocation =  _transform->_location;
	collision_mgr::instance().HitEffectPush(MyLocation + math::RandVec() * math::Rand<int>({ -65,65 }), 0.1f);
}

uint32_t ICE_Blast::get_layer_id() const&
{
	return layer_type::EObject;
}

std::shared_ptr<Bmp> ICE_Blast::Freezing::GetImg()
{
	static auto sp_Img = Bmp_mgr::instance().Find_Image_SP(L"ICE_BLAST");
	return sp_Img;
}

void ICE_Blast::Freezing::Freez(vec msgLocation)
{
	if (RemainTime > 0)return;

	object_mgr::instance().TextEffectMap[RGB(127, 255, 255)].
		push_back({ msgLocation,vec{0,1}*3,2.f,25,L"Freezing!" });
	
	RemainTime = Duration;
	ColIdx = 0;
	AnimTick = AnimDelta;
}


bool ICE_Blast::Freezing::update(float dt)
{
	RemainTime -= dt;
	if (RemainTime < 0)return false;
	
	AnimTick -= dt;
	if(AnimTick<0)
	{
		AnimTick = AnimDelta;
		++ColIdx;
		ColIdx = min(ColIdx, 8);
		if(RemainTime>0.23)
		{
			ColIdx = min(ColIdx, 2);
		}
	}
	return true;
}

void ICE_Blast::Freezing::render(HDC hdc, vec location, vec size)
{
	if (!IsFreezing())return;
	
	auto sp_Img = GetImg();
	if (!sp_Img)return;
	
	vec DestLoc = location -  ( size * 0.5f) ;

	GdiTransparentBlt(hdc, DestLoc.x, DestLoc.y, size.x, size.y,
		sp_Img->Get_MemDC(), PaintSize.x * ColIdx, 0,
		PaintSize.x, PaintSize.y, COLOR::MRGENTA());
}
