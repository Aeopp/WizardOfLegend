#include "pch.h"
#include "SwordMan.h"

#include "Bmp.h"

#include "Bmp_mgr.h"
#include "collision_mgr.h"
#include "render_component.h"
#include "Color.h"
#include "timer.h"

#include "object_mgr.h"
#include "SwordManAttack.h"
#include "game.h"
#include "Debuger.h"
#include "GoldEffect.h"
#include "sound_mgr.h"


void SwordMan::initialize()
{
	collision_lower_correction = { 0,+40 };

	lower_size = { 35,65 };

	LeftAnimKey = L"SWORDMAN_LEFT";
	RightAnimKey = L"SWORDMAN_RIGHT";

	_EnemyInfo.DeadTimer = 1.5f;
	_EnemyInfo.AttackRange = { 10,20 };
	_EnemyInfo.AttackStartDistance = 150.f;

	PaintSizeX = 200;
	PaintSizeY = 209; //  
	ScaleX = 0.8f;
	ScaleY = 0.8f;

	MyAnimDuration = 1.f;
	MyAnimInfo = {1,6,3,2,6};
	
	shadow_correction = {0,PaintSizeY *0.23f};
	ShadowWorldSizeCorrection = { 10,0 };
	/*void late_initialize(int ImgLocationX, int ImgLocationY,
		std::wstring ImgKey, layer_type layer_ID, int AnimColNum,
		int AnimRowIndex, float Duration, float AnimDuration,
		int PaintSizeX, int PaintSizeY, float ScaleX, float ScaleY);*/

	NormalAttack = object_mgr::instance().insert_object<SwordManAttack>(0, 0,
		L"__SWORDMAN_ATTACK", layer_type::EEffect, 4, 0, 1.f, 1.f, 200, 200, 1.0f, 1.0f);
	auto sp_NormalAttack = NormalAttack.lock();
	if (!sp_NormalAttack)return;
	sp_NormalAttack->_owner = _ptr;

	
	Attack = { 40,50 };
	


	// 필요한 정보들 미리 세팅 끝마치고호출 하기 바람
	Monster::initialize();

	auto sp_comp = _collision_component.lock();
	if (!sp_comp)return;
	sp_comp->bCollisionSlideAnObject = true;
	sp_comp->fSlideFactor = 1.f;
	_speed = 300.f;

	_EnemyInfo.HP = 450;
};

Event SwordMan::update(float dt)
{
	
	InitTime -= dt;
	AttackEndRemainTime -= dt;
	AttackStartTime -= dt;
	SoundTick -= dt;

	if (InitTime > 0)return Event::None;
	if (bDie)return Event::Die;
	if (bDying)return Event::None;
	Event _E = Monster::update(dt);
	if (_Freezing_Info.IsFreezing())return Event::None;

	
	if (AttackEndRemainTime > 0)
	{
		_render_component->_Anim.RowIndex = (int)EAnimState::Attack;
		_render_component->_Anim.ColIndex = 2;
		 return Event::None;
	}
	else
	{
		_render_component->_Anim.bOn = true;
	}

	if (AttackStartTime > 0)
	{
		_Shadow.CurrentShadowState = EShadowState::BIG;
		_render_component->_Anim.bOn = false;
		_render_component->_Anim.RowIndex = (int)EAnimState::Attack;
		_render_component->_Anim.ColIndex = 0;
		return Event::None;;
	}
	else if (_EnemyInfo.bAttack)
	{
		_EnemyInfo.bAttack = false;
		auto sp_Particle = NormalAttack.lock();
		if (!_EnemyInfo.bHit && sp_Particle)
		{
			sound_mgr::instance().Play("SWORDMAN_ATTACK", false, 1.f);

			sp_Particle->EffectStart(_transform->_dir, _transform->_location + _transform->_dir * 55.f);
			AttackEndRemainTime = 0.3f;
		}

	}

	if (!_EnemyInfo.bAttack)
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

	auto sp_Target =_AttackTarget.lock();
	if (!sp_Target)return Event::None;

	vec v = sp_Target->_transform->_location;
	vec w = _transform->_location;

	vec dis = v - w;
	vec dir = (dis.get_normalize());

	float distance = dis.length();
	float Attack_distance = _EnemyInfo.AttackStartDistance; 

	if (distance < Attack_distance && !_EnemyInfo.bHit && !_EnemyInfo.bAttack)
	{
		_EnemyInfo.bAttack = true;
		_transform->_dir = dir;
		AttackStartTime = 0.7f;
	}

	else if (Attack_distance<distance && !_EnemyInfo.bAttack && !_EnemyInfo.bHit)
	{
		vec rand_dir = dir;

		StalkerPosReTargetDuration -= dt;
		if (StalkerPosReTargetDuration < 0)
		{
			_transform->_dir = math::rotation_dir_to_add_angle(dir, math::Rand<float>({ -55,55 }));
			StalkerPosReTargetDuration = 1.f;
		}

		_transform->_location += _transform->_dir * dt * _speed;
		if (SoundTick < 0)
		{
			SoundTick = 0.8f;
			RAND_SOUNDPLAY("SWORDMAN_RUN", { 1,2 }, 1.f, false);
		}
		
		_render_component->ChangeAnim(EAnimState::Walk, 1.f);
		
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
	}
	else if (!_EnemyInfo.bHit && !_EnemyInfo.bAttack)
	{
		_Shadow.CurrentShadowState = EShadowState::NORMAL;
	}

	return  Event::None;

};
void SwordMan::Hit(std::weak_ptr<object>
	_target)
{
	if (InitTime > 0)return;
	
	Monster::Hit(_target);
	if (bInvincible)return;
	if (_EnemyInfo.bHit)return;
	if (bDying)return;
	auto sp_target = _target.lock();
	if (!sp_target)return;
	if (!sp_target->bAttacking)return;
	if (sp_target->ObjectTag == object::Tag::monster)return;
	//	if (sp_target->ObjectTag == object::Tag::player_shield)return;
	if (sp_target->ObjectTag == object::Tag::monster_attack)return;
	if (sp_target->UniqueID == EObjUniqueID::ICEBLAST && _Freezing_Info.IsFreezing())return;
	
	Timer::instance().event_regist(time_event::EOnce, HitCoolTime,
	[&bInvincible = bInvincible]()->bool {  bInvincible = false; return true;  });

	float Atk = math::Rand<int>(sp_target->Attack);

	bInvincible = true;
	_EnemyInfo.HP -= Atk;
	_EnemyInfo.bHit = true;
	_EnemyInfo.bAttack = false;
	AttackStartTime = AttackEndRemainTime = -1;


	HitSoundPlayBackByTag(sp_target->UniqueID, sp_target->ObjectTag);
	Hit_Calculation((int)EAnimState::Hit, sp_target->Attack);

	if (_EnemyInfo.HP < 0 &&   !_Freezing_Info.IsFreezing())
	{
		_render_component->_Anim.bOn = true;
		_render_component->ChangeAnim(EAnimState::Dead, 0.95, EAnimState::Dead);
		Monster::MonsterDie();
	};
}
void SwordMan::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (InitTime > 0)return;

	Monster::render(hdc, camera_pos, size_factor);
}

void SwordMan::AttackReadyCheck()
{
	if (AttackEndRemainTime > 0)
	{
		_render_component->_Anim.RowIndex = (int)EAnimState::Attack;
		_render_component->_Anim.ColIndex = 2;
		return;
	}
	else
	{
		_render_component->_Anim.bOn = true;
	}

	if (AttackStartTime > 0)
	{
		_Shadow.CurrentShadowState = EShadowState::BIG;
		_render_component->_Anim.bOn = false;
		_render_component->_Anim.RowIndex = (int)EAnimState::Attack;
		_render_component->_Anim.ColIndex = 0;
		return;
	}
	else
	{
		_EnemyInfo.bAttack = false;
		auto sp_Particle = NormalAttack.lock();
		if (!_EnemyInfo.bHit && sp_Particle)
		{
			sp_Particle->EffectStart(_transform->_dir,
			_transform->_location + _transform->_dir * 90.f);
			AttackEndRemainTime = 0.3f;
		}

	}
}

std::wstring SwordMan::GetSummonKey()
{
	return SwordMan::SummonCardImgKey;
}



