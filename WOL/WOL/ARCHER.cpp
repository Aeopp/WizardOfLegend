#include "pch.h"
#include "ARCHER.h"

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

void ARCHER::initialize()
{
	collision_lower_correction = { 0,+40 };

	lower_size = { 25,50 };

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
	EscapeRamainTick = EscapeDuration = 1.3f;
	_speed = 400.f;

	// 필요한 정보들 미리 세팅 끝마치고호출 하기 바람
	Monster::initialize();

}

Event ARCHER::update(float dt)
{
	
	InitTime -= dt;

	if (InitTime > 0)return Event::None;;

	if (bDie)
		return Event::Die;
	if (bDying)
		return Event::None;
	if (bArrowLineRender)
		AttackPreparationTime += dt;

	CurrentFireCoolTime -= dt;
	Event _E = Monster::update(dt);
	EscapeRamainTick -= dt;

	if (_EnemyInfo.bHit)
	{
		_render_component->ChangeAnim(EAnimState::Hit, 1.0f, EAnimState::Hit);
		return Event::None;
	}

	if (EscapeRamainTick >0)
	{
		_render_component->ChangeAnim(EAnimState::Walk, 0.6f);
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
		_transform->_location += EscapeVec * dt * _speed;
		return _E;
	}

	auto sp_Target = _AttackTarget.lock();
	if (!sp_Target)return Event::None;
	if (!NormalAttack)return Event::Die;
	

	vec v = sp_Target->_transform->_location;
	vec w = _transform->_location;

	vec dis = v - w;
	vec dir = (dis.get_normalize());

	float distance = dis.length();
	float Attack_distance = _EnemyInfo.AttackStartDistance;

	NormalAttack->_transform->_dir = dir;
	if (distance < Attack_distance  && !_EnemyInfo.bAttack)
	{
		_EnemyInfo.bAttack = true;
		NormalAttack->Preparation(true);
		SetArrowLineRender(true);

		_render_component->ChangeAnim(EAnimState::Attack, AttackPreparationTimeMax,EAnimState::Attack);

		sound_mgr::instance().Play("ARCHER_AIM", false, 1.f);
		
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
		// 여기서 공격 지점 표시 해줌
	
		// 여기서 공격
		if (CurrentFireCoolTime < 0)
		{
			Timer::instance().event_regist(time_event::EOnce, AttackPreparationTimeMax,
				[&bParticle = NormalAttack,
				&TargetLoc = sp_Target->_transform->_location,
				loc = _transform->_location,
				&_Shadow = _Shadow, &_Info = _EnemyInfo,
				&render = _render_component,
				&_NormalAttack = NormalAttack, 
				this,
				&_CoolTime = this->CurrentFireCoolTime](){

				if (!bParticle)return true;
				if (_Info.bHit)return true;
				if (!_Info.bAttack)return true;
				this->SetArrowLineRender(false); 

				_NormalAttack->Preparation(false);
				vec dir = (TargetLoc - loc).get_normalize();
				bParticle->launch(loc+dir*10.f,dir );
				_Shadow.CurrentShadowState = EShadowState::BIG;
				_Info.bAttack = false;
				sound_mgr::instance().Play("ARCHER_SHOOT", false, 1.f);
				return true;
			});				

			CurrentFireCoolTime = AttackPreparationTimeMax;

		}
		else if (CurrentFireCoolTime > 0)
		{
			_render_component->ChangeAnim(EAnimState::Attack, 0.3f);
			_Shadow.CurrentShadowState = EShadowState::NORMAL; 		
			SetArrowLineRender(false);
			 

			NormalAttack->Preparation(false);
		}
	}
	else if (Attack_distance < distance)
	{
		NormalAttack->Preparation(false);
		_EnemyInfo.bAttack = false;
		SetArrowLineRender(false);

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
	else if (!_EnemyInfo.bHit && !_EnemyInfo.bAttack)
	{
		_render_component->ChangeAnim(EAnimState::Idle, 0.3f);
		_Shadow.CurrentShadowState = EShadowState::NORMAL;
		NormalAttack->Preparation(false);		
		SetArrowLineRender(false);
	}

	return _E;
}

void ARCHER::Hit(std::weak_ptr<object> _target)
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
	NormalAttack->Preparation(false);
	_EnemyInfo.bHit = true;
	_EnemyInfo.bAttack = false;
	SetArrowLineRender(false);
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
		return true;});

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
		_render_component->ChangeUnstoppableAnim(EAnimState::Dead, 0.87f, EAnimState::Dead);
		MonsterDie();
	}
	else
	{
		SetArrowLineRender(false);
		NormalAttack->Preparation(false);
		EscapeVec = math::RandVec();
		EscapeRamainTick = EscapeDuration;
		_render_component->ChangeAnim(EAnimState::Walk, 0.6f);
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
	}
}
void ARCHER::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (InitTime > 0)return;
	if (!_transform)return;
	
	Monster::render(hdc, camera_pos, size_factor);

	

	if (bArrowLineRender)
	{
		auto sp_Target = _AttackTarget.lock();
		if (!sp_Target)return;

		vec MyLocation = _transform->_location - camera_pos;
		vec TargetLocation = sp_Target->_transform->_location - camera_pos;
		static float ColorConstant = 255 / AttackPreparationTimeMax;
		float CurrentColor = 255-(ColorConstant * AttackPreparationTime); 
		HPEN MyPen, OldPen;
		MyPen = CreatePen(PS_SOLID, 3, RGB(255, 
		CurrentColor , CurrentColor));
		OldPen = (HPEN)SelectObject(hdc, MyPen);

		MoveToEx(hdc, MyLocation.x, MyLocation.y, nullptr);
		LineTo(hdc, TargetLocation.x, TargetLocation.y);

		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);
	}
	
}
void ARCHER::SetArrowLineRender(bool SetFlag)
{
	bArrowLineRender = SetFlag;
	AttackPreparationTime = 0.0f;
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
}
std::wstring ARCHER::GetSummonKey()
{
	return ARCHER::SummonCardImgKey;
}
;
