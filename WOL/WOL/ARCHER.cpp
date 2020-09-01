#include "pch.h"
#include "ARCHER.h"

#include "Bmp_mgr.h"
#include "collision_mgr.h"
#include "render_component.h"
#include "Color.h"
#include "timer.h"
#include "object_mgr.h"
#include "ArcherArrow.h"
#include "ArcherBow.h"
#include "GoldEffect.h"

void ARCHER::initialize()
{
	collision_lower_correction = { 0,+40 };

	lower_size = { 20,45 };

	SummonCardImgKey = L"SUMMON_CARD_ARCHER";

	LeftAnimKey = L"ARCHER_LEFT";
	RightAnimKey = L"ARCHER_RIGHT";

	_EnemyInfo.HP = 200.f;
	_EnemyInfo.DeadTimer = 1.5f;
	_EnemyInfo.AttackRange = { 20,30 };
	_EnemyInfo.AttackStartDistance = 600.f;

	PaintSizeX = 200;
	PaintSizeY = 200;
	ScaleX = 0.8f;
	ScaleY = 0.8f;

	MyAnimDuration = 1.f;
	MyAnimInfo = { 1,5,4,2,6 };

	shadow_correction = { 0,PaintSizeY * 0.23f };
	ShadowWorldSizeCorrection = { 3,0 };
	/*void late_initialize(int ImgLocationX, int ImgLocationY,
		std::wstring ImgKey, layer_type layer_ID, int AnimColNum,
		int AnimRowIndex, float Duration, float AnimDuration,
		int PaintSizeX, int PaintSizeY, float ScaleX, float ScaleY);*/

	NormalAttack = object_mgr::instance().insert_object<ArcherBow>();
	NormalAttack->_owner = _ptr;
	NormalAttack->wp_Target = _AttackTarget;

	DefaultHitDuration = 0.15f;

	// 필요한 정보들 미리 세팅 끝마치고호출 하기 바람
	Monster::initialize();
}

Event ARCHER::update(float dt)
{
	if (bDie)
		return Event::Die;
	if (bDying)
		return Event::None;

	CoolTime -= dt;

	Event _E = Monster::update(dt);

	StateDuration -= dt;

	auto sp_Target = _AttackTarget.lock();
	if (!sp_Target)return Event::None;
	if (!NormalAttack)return Event::Die;

	vec v = sp_Target->_transform->_location;
	vec w = _transform->_location;

	vec dis = v - w;
	vec dir = (dis.get_normalize());

	float distance = dis.length();
	float Attack_distance = _EnemyInfo.AttackStartDistance;

	_transform->_dir = dir;
	NormalAttack->_transform->_dir = dir;
	if (distance < Attack_distance && !_EnemyInfo.bHit && !_EnemyInfo.bAttack)
	{
		_EnemyInfo.bAttack = true;
		NormalAttack->Preparation(true);

		_render_component->ChangeAnim(EAnimState::Attack, 3.f);
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
		// 여기서 공격
		if (CoolTime < 0)
		{
			Timer::instance().event_regist(time_event::EOnce, 3.f,
				[&bParticle = NormalAttack,
				&TargetLoc = sp_Target->_transform->_location,
				loc = _transform->_location,
				&_Shadow = _Shadow, &_Info = _EnemyInfo,
				&render = _render_component,
				&_NormalAttack = NormalAttack](){

				if (!bParticle)return true;
				if (_Info.bHit)return true;
				if (!_Info.bAttack)return true;

				_NormalAttack->Preparation(false);
				bParticle->launch(loc, (TargetLoc - loc).get_normalize());
				_Shadow.CurrentShadowState = EShadowState::BIG;
				_Info.bAttack = false;
				return true;
			});
			CoolTime = 3.f;
		}
	}
	else if (Attack_distance < distance && !_EnemyInfo.bHit)
	{
		NormalAttack->Preparation(false);
		_EnemyInfo.bAttack = false;
		_transform->_location += _transform->_dir * dt * _speed;
		_render_component->ChangeAnim(EAnimState::Walk, 0.6f);
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
	}
	else if (!_EnemyInfo.bHit && !_EnemyInfo.bAttack)
	{
		_Shadow.CurrentShadowState = EShadowState::NORMAL;
		NormalAttack->Preparation(false );
	}

	return _E;
}

void ARCHER::Hit(std::weak_ptr<object> _target)
{
	Monster::Hit(_target);

	if (bDying)return;
	auto sp_target = _target.lock();
	if (!sp_target)return;
	if (!sp_target->bAttacking)return;
	if (bInvincible)return;
	Timer::instance().event_regist(time_event::EOnce, 0.2f,
		[&bInvincible = bInvincible]()->bool
		{  bInvincible = false; return true;  });

	Timer::instance().event_regist(time_event::EOnce, 0.2,
		[&bHit = _EnemyInfo.bHit](){
		bHit = false;
		return true;
	});
	NormalAttack->Preparation(false);

	bInvincible = true;
	StateDuration = 0.2;
	CurrentState = EMonsterState::Hit;

	_EnemyInfo.bAttack = false;
	_render_component->ChangeAnim(EAnimState::Hit, 0.2);
	_Shadow.CurrentShadowState = EShadowState::BIG;

	float Atk = sp_target->Attack;
	_EnemyInfo.HP -= Atk;
	_EnemyInfo.bHit = true;

	vec randvec = math::RandVec();
	randvec.y = (abs(randvec.y));
	vec v = _transform->_location;
	v.y -= 35;
	v.x += math::Rand<int>({ -40,+40 });
	Atk = math::Rand<int>({ -20,20 });
	int Size = 23;

	object_mgr::instance().TextEffectMap[RGB(221, 221, 221)].
		push_back({ v ,vec{0,1}*3,
		1.f,Size,std::to_wstring((int)Atk) });

	if (_EnemyInfo.HP < 0)
	{
		bDying = true;
		CurrentState = EMonsterState::Dead;

		_render_component->ChangeUnstoppableAnim(EAnimState::Dead, 1.f, EAnimState::Dead);

		Timer::instance().event_regist(time_event::EOnce, 1,

			[AttackTarget = _AttackTarget, v = _transform->_location, &bDie = this->bDie]()
		{bDie = true;

		auto _gold = GoldEffect::MakeGold(v.x, v.y,
			L"MONEY", layer_type::EEffect, 2,
			math::Rand<int>({ 1,3 }), FLT_MAX, 0.2f, 24, 24, 1.f, 1.f, AttackTarget);
		return true;
		});
	};
}
void ARCHER::render(HDC hdc, vec camera_pos, vec size_factor)
{
	Monster::render(hdc, camera_pos, size_factor);
}
void ARCHER::DirCheckAnimFileChange()
{
	vec dir =_transform->_dir;
	dir = dir.get_normalize();

	if (dir.x < 0)
	{
		_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(
		LeftAnimKey);
	}
	else
	{
		_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(
			RightAnimKey);
	}
};
