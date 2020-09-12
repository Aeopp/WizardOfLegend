#include "pch.h"
#include "Monster.h"
#include "collision_mgr.h"
#include "object_mgr.h"
#include "timer.h"
#include "render_component.h"
#include "Color.h"
#include "Bmp_mgr.h"
#include "Effect.h"
#include "GoldEffect.h"
#include "sound_mgr.h"


#include "game.h"
#include "player_info.h"

void Monster::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!_transform)return;
	
	Freezing_render(hdc, _transform->_location - camera_pos);
	
	if (!_Freezing_Info.IsFreezing())
	{
		actor::render(hdc, camera_pos, size_factor);
		Burning_render(hdc, _transform->_location - camera_pos);
	}
}

void Monster::CheckDirChangeImgFile()
{
	vec dir = _transform->_dir;
	float scala = dir.dot(vec{ 1,0 });


	if (scala > 0)
	{
		_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(RightAnimKey);
	}
	else
	{
		_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(LeftAnimKey);
	}
}


void Monster::initialize()
{
	actor::initialize();
	
	_collision_component = collision_mgr::instance().insert
	(_ptr, collision_tag::EMonster, ERect);
	auto sp_collision_lower = _collision_component.lock();

	if (!sp_collision_lower)return;

	collision_lower_correction = lower_correction;

	sp_collision_lower->_size = lower_size;
	sp_collision_lower->bSlide = true;
	sp_collision_lower->bHitEffect = false;
	sp_collision_lower->bCollisionSlideAnObject = true;
	sp_collision_lower->bCollision = true;

	if(_transform)
	_transform->_location = { 0.f,0.f };

	bAttacking = true;

	_render_component = render_component::LoadRenderComponent_SP
	(LeftAnimKey+L".bmp", LeftAnimKey);
	_render_component->Default_Src_Paint_Size = vec{ PaintSizeX,PaintSizeY };
	_render_component->Dest_Paint_Size = vec{ PaintSizeX * ScaleX,PaintSizeY * ScaleY};
	_render_component->_ColorKey = COLOR::MRGENTA();
	_render_component->_Img_src = RECT{ 0,0,PaintSizeX,PaintSizeY };
	_render_component->_Anim.SetAnimationClip(
		MyAnimInfo, MyAnimDuration);

	_Shadow.correction = shadow_correction;
	_Shadow.world_size_correction = ShadowWorldSizeCorrection;
	ObjectTag = object::Tag::monster;

	InitTime = 3.5f;
};

Event Monster::update(float dt)
{
	Event _E = actor::update(dt);

	CheckDirChangeImgFile();
	Freezing_update(dt, _collision_component);
	burning_update(dt);
	
	return _E;
};

void Monster::Hit(std::weak_ptr<object> _target)
{
	actor::Hit(_target);
}

void Monster::late_initialize(std::weak_ptr<class object> SetTarget, vec SetLocation)
{
	_AttackTarget = (SetTarget);
	if(_transform)
	_transform->_location = (SetLocation); 

	CardEffect(SetLocation,GetSummonKey());
}

void Monster::HitSoundPlayBackByTag(EObjUniqueID CheckID , object::Tag CheckTag)
{
	if (CheckTag == object::Tag::player_shield)
		RAND_SOUNDPLAY("GAIA_ARMOR_HIT", { 1,4 });
	else if (CheckID == EObjUniqueID::ICEBLAST)
		ICEHitSoundPlay();
	else if (CheckID == EObjUniqueID::ICECRYSTAL)
		WINDHitSoundPlay();
	else if (CheckID == EObjUniqueID::Boomerang)
		BoomerangSoundPlay();
	else if (CheckID == EObjUniqueID::FIREDRAGON)
		BoomerangSoundPlay();
	else 
		NormalHitSoundPlay();

	RAND_SOUNDPLAY("HIT_SOUND_NORMAL", { 1,2 }, 1.f, false);
}

void Monster::DieSoundPlay()
{
	sound_mgr::instance().RandSoundKeyPlay("ENEMY_DIED", { 0,3 }, 1.f);
}

void Monster::MonsterDie()
{
	bDying = true;

	Timer::instance().event_regist(time_event::EOnce, 0.8,
		[this]()
	{
		this->bDie = true;
		DieSoundPlay();
		auto _gold = GoldEffect::MakeGold(_transform->_location.x, 
										  _transform->_location.y,
			L"MONEY", layer_type::EEffect, 2,
			math::Rand<int>({ 0,2 }), FLT_MAX,
			0.5f, 24, 24, 1.f, 1.f, _AttackTarget);

		return true;
	});

	
	auto sp_col = _collision_component.lock();
	if (!sp_col)return;
	sp_col->bDie = true;
}



void Monster::ICEHitSoundPlay()
{
	sound_mgr::instance().RandSoundKeyPlay("ENEMY_HITTED_ICE", { 1,4 },1.f);
}

void Monster::WINDHitSoundPlay()
{
	sound_mgr::instance().RandSoundKeyPlay("ENEMY_HITTED_WIND", { 1,3 },1.f);
}

void Monster::NormalHitSoundPlay()
{
	sound_mgr::instance().Play("ENEMY_HITED_1");
}

void Monster::BoomerangSoundPlay()
{
	RAND_SOUNDPLAY("FIRE_DRAGON_HITTED_ENEMY", { 1,6 }, 1.f, false);
}

void Monster::Hit_Calculation(int HitAnimIdx , std::pair<int,int> AttackRange)
{
	_render_component->ChangeAnim(HitAnimIdx, HitCoolTime);
	_Shadow.CurrentShadowState = EShadowState::BIG;
	collision_mgr::instance().HitEffectPush(_transform->_location, HitCoolTime);

	float Atk = math::Rand<int>(AttackRange);
	COLORREF _HitEffectColor = RGB(221, 221, 221);
	std::wstring Msg = std::to_wstring((int)Atk); 
	// 속성 상태일시 피해증폭

	if (_Freezing_Info.IsFreezing())
	{
		Atk *= Freezing_Interface::Amplification_factor;
		_HitEffectColor = Freezing_Interface::EffectColor;
		Msg += L"!";
	}
	else if(_Burning_Info.IsBurning())
	{
		Atk *= Burning_Interface::Amplification_factor;
		_HitEffectColor = Burning_Interface::EffectColor;
		Msg += L"!";
	}
	
	_EnemyInfo.HP -= Atk;

	Timer::instance().event_regist(time_event::EOnce, HitCoolTime,
		[&bInvincible = bInvincible]()->bool
	{  bInvincible = false; return true;  });

	Timer::instance().event_regist(time_event::EOnce, HitCoolTime,
		[&bHit = _EnemyInfo.bHit](){
		bHit = false;
		return true; });

	vec randvec = math::RandVec();
	randvec.y = (abs(randvec.y));
	vec v = _transform->_location;
	v.y -= 35;
	v.x += math::Rand<int>({ -40,+40 });

	object_mgr::instance().TextEffectMap[_HitEffectColor].
		push_back({ v ,vec{0,1}*3,
		1.f,int(Atk),  std::move( Msg  ) });

	Monster::MonsterHitPlayerSignatureGageAdd(Atk);
}


void Monster::MonsterHitPlayerSignatureGageAdd(float Atk)
{
	auto sp_PInfo = game::instance()._player_info;
	if (!sp_PInfo)return;
	
	sp_PInfo->SignatureGaugeAdd(Atk);
}

void Monster::CardEffect(vec v,std::wstring ImageKey)
{
	object_mgr::instance().insert_object<Effect>(v.x,
		v.y,ImageKey,layer_type::EEffect,
		32,0,4.0f,3.0f,150,230,1.0f,1.0f);

	sound_mgr::instance().Play("CARD_SUMMON", false, 1.f);
};


