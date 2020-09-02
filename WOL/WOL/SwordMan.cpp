#include "pch.h"
#include "SwordMan.h"

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

	lower_size = { 20,45 };


	LeftAnimKey = L"SWORDMAN_LEFT";
	RightAnimKey = L"SWORDMAN_RIGHT";

	_EnemyInfo.HP = 100.f;
	_EnemyInfo.DeadTimer = 15.f;
	_EnemyInfo.AttackRange = { 10,20 };
	_EnemyInfo.AttackStartDistance = 170.f;

	PaintSizeX = 200;
	PaintSizeY = 209;
	ScaleX = 0.7f;
	ScaleY = 0.7f;

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

	DefaultHitDuration = 0.25f;
	Attack = { 40,50 };
	InitTime = 4.7f;

	InvincibleTime = 0.3f;

	// 필요한 정보들 미리 세팅 끝마치고호출 하기 바람
	Monster::initialize();
};

Event SwordMan::update(float dt)
{
	InitTime -= dt;
	if (bDie)
	{
		return Event::Die;
	}
	if (bDying)return Event::None;
	if(InitTime>0)return Event::None;
	
	StateDuration -= dt;

	Event _E = Monster::update(dt);
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
		_render_component->ChangeAnim(EAnimState::Attack, 0.6f);
		_render_component->_Anim.ColIndex = 0;
		_render_component->_Anim.bOn = false;
		_Shadow.CurrentShadowState = EShadowState::BIG;
		// 여기서 공격
		Timer::instance().event_regist(time_event::EOnce,0.6f,
		[&bOn = _render_component->_Anim.bOn,
		&bParticle = NormalAttack,dir = _transform->_dir,loc = _transform->_location,
		&_Shadow = _Shadow](){
			auto sp_Particle = bParticle.lock();
			if (!sp_Particle)return true;
			sp_Particle->EffectStart(dir,loc+dir*90.f);
		
			_Shadow.CurrentShadowState = EShadowState::BIG;
			bOn = true;
			return true; 
			});

		Timer::instance().event_regist(time_event::EOnce, 1.2f,
			[&bAttack = _EnemyInfo.bAttack](){
			bAttack = false;
			return true;
		});
	}
	else if (Attack_distance<distance && !_EnemyInfo.bHit && !_EnemyInfo.bAttack)
	{
		vec rand_dir = dir;

		StalkerDuration -= dt;
		if (StalkerDuration < 0)
		{
			_transform->_dir = math::rotation_dir_to_add_angle(dir, math::Rand<float>({ -55,55 }));
			StalkerDuration = 1.f;
		}
		_transform->_location += _transform->_dir * dt * _speed;
		_render_component->ChangeAnim(EAnimState::Walk, 1.f);
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
	}
	else if (!_EnemyInfo.bHit && !_EnemyInfo.bAttack)
	{
		_Shadow.CurrentShadowState = EShadowState::NORMAL;
	}

	return _E;
};
void SwordMan::Hit(std::weak_ptr<object> _target)
{
	if (InitTime > 0)return;
	
	Monster::Hit(_target);

	if (bDying)return;
	auto sp_target = _target.lock();
	if (!sp_target)return;
	if (!sp_target->bAttacking)return;
	//if (sp_target->ObjectTag == object::Tag::player_shield)return;
	if (sp_target->ObjectTag == object::Tag::monster)return;
	if (sp_target->ObjectTag == object::Tag::monster_attack)return;
	if (bInvincible)return;
	
	if (sp_target->UniqueID == EobjUniqueID::NormalAttack)
		sound_mgr::instance().RandSoundKeyPlay("HIT_SOUND_NORMAL", { 1,2 }, 1.f);


	Timer::instance().event_regist(time_event::EOnce, InvincibleTime,
	[&bInvincible = bInvincible]()->bool {  bInvincible = false; return true;  });

	bInvincible = true;
	StateDuration = DefaultHitDuration;
	CurrentState = EMonsterState::Hit;

	float Atk = math::Rand<int>(sp_target->Attack);
	_EnemyInfo.HP -= Atk;
	_EnemyInfo.bHit = true;

	vec randvec = math::RandVec();
	randvec.y = (abs(randvec.y));
	vec v = _transform->_location;
	v.y -= 35;
	v.x += math::Rand<int>({ -40,+40 });

	object_mgr::instance().TextEffectMap[RGB(221, 221, 221)].
		push_back({ v ,vec{0,1}*3,
		   1.f,int(Atk),std::to_wstring((int)Atk) });

	Timer::instance().event_regist(time_event::EOnce, DefaultHitDuration,
		[&bHit = _EnemyInfo.bHit](){
		bHit = false;
		return true;
	});

	_render_component->ChangeAnim(EAnimState::Hit, DefaultHitDuration);
	_Shadow.CurrentShadowState = EShadowState::BIG;
	collision_mgr::instance().HitEffectPush(_transform->_location, 0.3f);

	if (_EnemyInfo.HP < 0)
	{
		bDying = true;
		CurrentState = EMonsterState::Dead;
		_render_component->ChangeUnstoppableAnim(EAnimState::Dead, 0.8f, EAnimState::Dead);

		Timer::instance().event_regist(time_event::EOnce, 1,

			[AttackTarget = _AttackTarget, v = _transform->_location, &bDie = this->bDie]()
		{
			bDie = true;

			auto _gold = GoldEffect::MakeGold(v.x, v.y,
				L"MONEY", layer_type::EEffect, 2,
				math::Rand<int>({ 0,2 }), FLT_MAX, 0.5f, 24, 24, 1.f, 1.f, AttackTarget);
			return true;
		});

		auto sp_col = _collision_component.lock();
		if (!sp_col)return;
		sp_col->bDie = true;
	};
}
void SwordMan::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (InitTime > 0)return;

	Monster::render(hdc, camera_pos, size_factor);
}



