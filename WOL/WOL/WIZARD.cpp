#include "pch.h"
#include "WIZARD.h"

#include "sound_mgr.h"

#include "Bmp_mgr.h"
#include "collision_mgr.h"
#include "render_component.h"
#include "Color.h"
#include "timer.h"
#include "object_mgr.h"
#include "ArcherArrow.h"
#include "ArcherBow.h"
#include "GoldEffect.h"
#include "WizardFire.h"
#include "WizardBall.h"

void WIZARD::initialize()
{
	collision_lower_correction = { 0,+40 };

	lower_size = { 25,50 };

	LeftAnimKey = L"WIZARD_LEFT";
	RightAnimKey = L"WIZARD_RIGHT";

	_EnemyInfo.HP = 200.f;
	_EnemyInfo.DeadTimer = 1.5f;
	_EnemyInfo.AttackRange = { 20,30 };
	_EnemyInfo.AttackStartDistance = 600.f;

	PaintSizeX = 170;
	PaintSizeY = 230;
	ScaleX = 0.8f;
	ScaleY = 0.8f;

	MyAnimDuration = 1.f;
	MyAnimInfo = { 1,4,4,2,4 };

	shadow_correction = { 0,PaintSizeY * 0.4f };
	ShadowWorldSizeCorrection = { 20,0 };
	/*void late_initialize(int ImgLocationX, int ImgLocationY,
		std::wstring ImgKey, layer_type layer_ID, int AnimColNum,
		int AnimRowIndex, float Duration, float AnimDuration,
		int PaintSizeX, int PaintSizeY, float ScaleX, float ScaleY);*/


	DefaultHitDuration = 0.15f;
	EscapeRamainTick = EscapeDuration = 1.3f;
	_speed = 200.f;

	FireImg = Bmp_mgr::instance().Find_Image_SP(L"WIZARD_FIRE");

	// 필요한 정보들 미리 세팅 끝마치고호출 하기 바람
	Monster::initialize();
}

Event WIZARD::update(float dt)
{
	InitTime -= dt;
	if (InitTime > 0)return Event::None;

	if (bDie)
		return Event::Die;
	if (bDying)
		return Event::None;

	CurrentBallCoolTime -= dt;
	CurrentRandMoveDuration -= dt;
	CurrentFireCoolTime -= dt;
	Event _E = Monster::update(dt);
	EscapeRamainTick -= dt;

	if (_EnemyInfo.bHit)
	{
		_render_component->ChangeAnim(EAnimState::Hit, 1.0f, EAnimState::Hit);
		return Event::None;
	}

	if (EscapeRamainTick > 0)
	{
		_render_component->ChangeAnim(EAnimState::Walk, 0.6f);
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
		_transform->_location += EscapeVec * dt * _speed;
		return _E;
	}

	auto sp_Target = _AttackTarget.lock();
	if (!sp_Target)return Event::None;


	vec TargetLocation = sp_Target->_transform->_location;
	vec MyLocation = _transform->_location;

	vec dis = TargetLocation - MyLocation;
	vec dir = (dis.get_normalize());

	float distance = dis.length();
	float Attack_distance = _EnemyInfo.AttackStartDistance;

	if (distance < Attack_distance && !_EnemyInfo.bAttack)
	{

		_render_component->ChangeAnim(EAnimState::Attack, 2.3f, EAnimState::Attack);


		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
		// 여기서 공격
		if (CurrentFireCoolTime < 0)
		{
			_EnemyInfo.bAttack = true;

			Timer::instance().event_regist(time_event::EOnce, 2.3f,
				[dir,this](){
				if (_EnemyInfo.bHit)return true;
				if (!_EnemyInfo.bAttack)return true;
				if (!FireImg)return true;
				auto NormalAttack = object_mgr::instance().insert_object<WizardFire>();
				if (!NormalAttack)return true;
				NormalAttack->_owner = _ptr;
				NormalAttack->wp_Target = _AttackTarget;
				NormalAttack->Cast(_transform->_location +  math::RandVec() * math::Rand<int>({ 20,200}), dir, 0, FireImg);
				_Shadow.CurrentShadowState = EShadowState::BIG;
				_EnemyInfo.bAttack = false;


				return true;
			});
			
			
			CurrentFireCoolTime = FireCoolTime;
		}
		else if (CurrentFireCoolTime > 0)
		{
			

			_render_component->ChangeAnim(EAnimState::Attack, 0.3f);
			_Shadow.CurrentShadowState = EShadowState::NORMAL;

		}

		if (CurrentBallCoolTime < 0)
		{
			_EnemyInfo.bAttack = true;
			CurrentBallCoolTime = BallCoolTime;

			for (int i = 0; i < 3; ++i)
			{
				vec InitPos = _transform->_location +
					math::RandVec() * math::Rand<float>({ -400,400});
				object_mgr::instance().insert_object<WizardBall>(_AttackTarget, InitPos);
			}

			Timer::instance().event_regist(time_event::EOnce, 2.3f,
				[&EInfo = _EnemyInfo]() {
					EInfo.bAttack = false;
					return true; });
		}
	}
	else if (Attack_distance < distance)
	{
		//NormalAttack->Preparation(false);
		_EnemyInfo.bAttack = false;

		StalkerPosReTargetDuration -= dt;
		if (StalkerPosReTargetDuration < 0)
		{
			_transform->_dir = math::rotation_dir_to_add_angle(dir, math::Rand<float>({ -89,89 }));
			StalkerPosReTargetDuration = 1.5f;
		}
		_transform->_location += _transform->_dir * dt * _speed;
		_render_component->ChangeAnim(EAnimState::Walk, 0.6f);
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
	}
	else if (Attack_distance > distance &&!_EnemyInfo.bHit && !_EnemyInfo.bAttack && CurrentRandMoveDuration>0)
	{
		_render_component->ChangeAnim(EAnimState::Walk, 0.3f);
		_Shadow.CurrentShadowState = EShadowState::NORMAL;
		_transform->_location += RandMoveVec* dt * _speed;
	}
	if (CurrentRandMoveDuration < 0) {
		 CurrentRandMoveDuration = RandMoveVecDuration;
		 RandMoveVec = math::RandVec();
	 }
	//RandMoveVec = vec{ 0,0 };
	return _E;
}

void WIZARD::Hit(std::weak_ptr<object> _target)
{

	if (InitTime > 0)return;
	Monster::Hit(_target);
	if (bInvincible)return;
	if (bDying)return;
	auto sp_target = _target.lock();
	if (!sp_target)return;
	if (!sp_target->bAttacking)return;
	//	if (sp_target->ObjectTag == object::Tag::player_shield)return;
	if (sp_target->ObjectTag == object::Tag::monster)return;
	if (sp_target->ObjectTag == object::Tag::monster_attack)return;

	HitSoundPlayBackByTag(sp_target->UniqueID, sp_target->ObjectTag);

	bInvincible = true;
	_EnemyInfo.bHit = true;
	_EnemyInfo.bAttack = false;

	_render_component->ChangeAnim(EAnimState::Hit, 0.4f);
	_Shadow.CurrentShadowState = EShadowState::BIG;
	collision_mgr::instance().HitEffectPush(_transform->_location, 0.5f);

	float Atk = math::Rand<int>(sp_target->Attack);
	_EnemyInfo.HP -= Atk;

	Timer::instance().event_regist(time_event::EOnce, 0.3f,
		[&bInvincible = bInvincible]()->bool
		{  bInvincible = false; return true;  });

	Timer::instance().event_regist(time_event::EOnce, 0.3f,
		[&bHit = _EnemyInfo.bHit](){
		bHit = false;
		return true; });

	vec randvec = math::RandVec();
	randvec.y = (abs(randvec.y));
	vec v = _transform->_location;
	v.y -= 35;
	v.x += math::Rand<int>({ -40,+40 });

	object_mgr::instance().TextEffectMap[RGB(221, 221, 221)].
		push_back({ v ,vec{0,1}*3,
		1.f,int(Atk),std::to_wstring((int)Atk) });

	if (_EnemyInfo.HP < 0)
	{


		_render_component->ChangeUnstoppableAnim(EAnimState::Dead, 0.8f, EAnimState::Dead);
		MonsterDie();
	}
	else
	{
		//NormalAttack->Preparation(false);
		EscapeVec = math::RandVec();
		EscapeRamainTick = EscapeDuration;
		_render_component->ChangeAnim(EAnimState::Walk, 0.6f);
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
	}
}
void WIZARD::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (InitTime > 0)return;


	Monster::render(hdc, camera_pos, size_factor);
}
void WIZARD::FireCast()
{
}
void WIZARD::DirCheckAnimFileChange()
{
	vec dir = _transform->_dir;
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
}
std::wstring WIZARD::GetSummonKey()
{
	return WIZARD::SummonCardImgKey;
}
;
