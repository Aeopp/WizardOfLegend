#include "pch.h"
#include "WizardBall.h"
#include "collision_component.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "collision_mgr.h"
#include "Shadow.h"
#include "timer.h"
#include "object_mgr.h"
#include "shield.h"
#include "sound_mgr.h"
#include "Monster.h"



void WizardBall::initialize()
{
	object::initialize();

	sp_Bmp = Bmp_mgr::instance().Find_Image_SP(L"WIZARD_BALL");

	wp_collision =collision_mgr::instance().
	insert(_ptr, collision_tag::EMonsterAttack,figure_type::ECircle);

	auto sp_collision = wp_collision.lock();

	if (!sp_collision)return;

	this->bAttacking = false;
	sp_collision->bCollision = true;
	sp_collision->bCollisionSlideAnObject = false;
	sp_collision->bCollisionTargetPushFromForce = false;
	sp_collision->bHitEffect = true;
	sp_collision->bRender = true;
	sp_collision->bSlide= true;
	sp_collision->PushForce = 1.f;
	sp_collision->_size = {30.f,30.f};

	_Shadow.initialize();
	_Shadow._owner = (_ptr);
	_Shadow.correction = { 0,150 };


	PaintSizeX = 90;
	PaintSizeY = 90;
	ScaleX = 0.75f;
	ScaleY = 0.75f;
	Speed = 150.f;
	AttackSpeed = 1500.f;
	Attack = { 40,50 };
	AttackStartDistance = 750.f;
	UniqueID = EObjUniqueID::EWizardBall;
	ObjectTag = Tag::monster_attack; 


}

void WizardBall::late_initialize(std::weak_ptr<class object> wp_AttackTarget, vec location)
{
	this->_transform->_location = std::move(location);
	this->wp_AttackTarget = std::move(wp_AttackTarget);

	Monster::CardEffect(location, WizardBall::SummonCardImgKey);
}

void WizardBall::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (InitTime > 0)return; 
	_Shadow.render(hdc, camera_pos);

	// 애니메이션 정보 세팅
	AnimUpdateFromCurrentState();

	 vec DestPaintSize = { PaintSizeX * ScaleX, PaintSizeY * ScaleY }; 
	 vec DestLoc = _transform->_location - camera_pos - (DestPaintSize*0.5);

	 GdiTransparentBlt(hdc,
			  DestLoc.x, DestLoc.y
			 , DestPaintSize.x, DestPaintSize.y
			 , sp_Bmp->Get_MemDC(), 
			  CurrentColIdx * PaintSizeX,
			   CurrentRowIdx*PaintSizeY ,
			  PaintSizeX, PaintSizeY, 
			COLOR::MRGENTA());
}

Event WizardBall::update(float dt)
{
	Event _Event= object::update(dt);
	InitTime -= dt;

	if (InitTime > 0)
		return _Event;

	StateDuration -= dt;
	CurrentHitCoolTime -= dt;
	
	UpdateDir();
	StateAction();
	StateTranslation();

	return _Event;  
}

void WizardBall::Hit(std::weak_ptr<object> _target)
{
	if (InitTime > 0)return;

	auto sp_Target = _target.lock();
	if (!sp_Target)return;
	float Atk = math::Rand<float>(sp_Target->Attack);

	if (sp_Target->ObjectTag == object::Tag::player_attack && CurrentHitCoolTime<0)
	{
		CurrentHitCoolTime = 0.1f;
		vec randvec = math::RandVec();
		randvec.y = (abs(randvec.y));
		vec v = _transform->_location;
		v.y -= 35;
		v.x += math::Rand<int>({ -40,+40 });

		object_mgr::instance().TextEffectMap[RGB(221, 221, 221)].
			push_back({ v ,vec{0,1}*3,
			1.f,int(Atk),std::to_wstring((int)Atk) });

		RAND_SOUNDPLAY("HIT_SOUND_NORMAL", { 1,2 }, 1.f, false);

		collision_mgr::instance().HitEffectPush(_transform->_location, 0.5f);
		CurrentState = WizardBall::EState::HIT;
		StateDuration = 0.2f;

		HP -= Atk;

		if (HP < 0)
			bDie = true;
	}
	else if (sp_Target->ObjectTag == object::Tag::player_shield && CurrentHitCoolTime < 0)
	{
		collision_mgr::instance().HitEffectPush(_transform->_location, 0.5f);
		CurrentState = WizardBall::EState::HIT;
		StateDuration = 0.2f;
		CurrentHitCoolTime = 0.1f;
		RAND_SOUNDPLAY("HIT_SOUND_NORMAL", { 1,2 }, 1.f, false);
		if (bAttacking && !bDef)
		{
			bDef = true;
			shield::DefenseMsg(_transform->_location);
		}
	}

	Monster::MonsterHitPlayerSignatureGageAdd(Atk);

}



void WizardBall::AnimColUpdate(bool Updown)
{
	CurrentAnimDelta -= DeltaTime;
	if (CurrentAnimDelta < 0)
	{
		CurrentAnimDelta = AnimDelta;
		if (Updown)
			CurrentColIdx++;
		else
			CurrentColIdx--;

		CurrentColIdx = std::clamp<>(CurrentColIdx, 0, 3);
	}
};

void WizardBall::AnimUpdateFromCurrentState()
{
	switch (CurrentState)
	{
	case WizardBall::EState::BALL:
		CurrentRowIdx = 0;
		CurrentColIdx = 0;
		break;
	case WizardBall::EState::AttackReady:
		CurrentRowIdx = AnimRowCalcFromAngle();
		AnimColUpdate(true);
		break;
	case WizardBall::EState::Attacking:
		CurrentRowIdx = AnimRowCalcFromAngle();
		break;
	case  WizardBall::EState::AttackEnd:
		CurrentRowIdx = AnimRowCalcFromAngle();
		AnimColUpdate(false);
		break;
	case WizardBall::EState::HIT:
		CurrentColIdx = 1;
		CurrentRowIdx = 0;
		break;
	default:
		break;
	}
}

int WizardBall::AnimRowCalcFromAngle()
{
	if (!_transform)return 0;

	vec CurrentDir = _transform->_dir;
	float Angle360 = math::AngleFromVec(CurrentDir);
	int RowIDX = std::round((360 - Angle360) / 22.5f);
	RowIDX = std::clamp<>(RowIDX, 1, 16);
	return RowIDX;
}

void WizardBall::StateTranslation()
{
	if (StateDuration > 0)return;

	switch (CurrentState)
	{
	case WizardBall::EState::BALL:
		CurrentState = WizardBall::EState::AttackReady;
		AttackReady();
		StateDuration = 1.5;
		break;
	case WizardBall::EState::AttackReady:
		CurrentState = WizardBall::EState::Attacking;
		StateDuration = 0.5f;
		AttackStart();
		break;
	case WizardBall::EState::Attacking:
		CurrentState = WizardBall::EState::AttackEnd;
		StateDuration = 1.0;
		AttackEnd();
		break;
	case WizardBall::EState::AttackEnd:
		CurrentState = WizardBall::EState::BALL;
		StateDuration = 1.f;
		break;
	case WizardBall::EState::HIT:
		CurrentState = WizardBall::EState::BALL;
		StateDuration = 0.5f;
		break;
	default:
		break;
	}
}

void WizardBall::AttackStart()
{
	CurrentColIdx = 3;
	sound_mgr::instance().Play("BALL_ATTACK",false ,1.f);

	auto sp_AttackTarget = wp_AttackTarget.lock();
	if (!sp_AttackTarget)return;
	vec TargetLocation = sp_AttackTarget->_transform->_location;
	vec Dis = TargetLocation - _transform->_location;
	vec Dir = Dis.get_normalize();
	AttackDir = Dir;

	bAttacking = true;
}

void WizardBall::StateAction()
{
	switch (CurrentState)
	{
	case WizardBall::EState::BALL:
		IdleAction();
		break;
	case WizardBall::EState::AttackReady:
		break;
	case WizardBall::EState::Attacking:
		Attacking();
		break;
	case WizardBall::EState::AttackEnd:
		break;
	case WizardBall::EState::HIT:
		break;
	default:
		break;
	}
}

void WizardBall::UpdateDir()
{

	auto sp_AttackTarget = wp_AttackTarget.lock();
	if (!sp_AttackTarget)return;
	vec TargetLocation = sp_AttackTarget->_transform->_location;
	vec Dis = TargetLocation - _transform->_location;
	vec Dir = Dis.get_normalize();
	_transform->_dir = Dir;

}

void WizardBall::ConditionCheck()
{
	

}

bool WizardBall::Attacking()
{
	if (!_transform)return true;

	_transform->_location += AttackDir* AttackSpeed *DeltaTime;
	return true;
}

bool WizardBall::AttackEnd()
{
	bAttacking = false;
	CurrentColIdx = 3;

	return true;
}

void WizardBall::AttackReady()
{
	bDef = false;
	sound_mgr::instance().Play("BALL_ATTACKMODE",false ,1.f);
	CurrentColIdx = 0;
}

void WizardBall::IdleAction()
{
	auto sp_AttackTarget = wp_AttackTarget.lock();
	if (!sp_AttackTarget)return;
	if (!_transform)return;

	vec TargetLocation = sp_AttackTarget->_transform->_location;
	vec Dis = TargetLocation - _transform->_location;
	vec Dir = Dis.get_normalize();
	float Distance = Dis.length();

	_transform->_dir = Dir;

	if (Distance > AttackStartDistance)
	{
		_transform->Move(_transform->_dir, Speed * DeltaTime);
		StateDuration += DeltaTime;
	}
}
