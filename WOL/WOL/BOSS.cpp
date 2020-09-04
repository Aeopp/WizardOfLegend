#include "pch.h"
#include "UIBossHPBar.h"
#include "BOSS.h"
#include "helper.h"
#include "BossInfo.h"
#include "collision_component.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "collision_mgr.h"
#include "Shadow.h"
#include "timer.h"
#include "object_mgr.h"
#include "shield.h"
#include "sound_mgr.h"
#include "UIBossName.h"

void BOSS::initialize()
{
	object::initialize();

	sp_Bmps.first = Bmp_mgr::instance().Find_Image_SP(L"BOSS_LEFT");
	sp_Bmps.second = Bmp_mgr::instance().Find_Image_SP(L"BOSS_RIGHT");
	sp_AttackBmp = Bmp_mgr::instance().Find_Image_SP(L"BOSS_ATTACK");

	wp_collision = collision_mgr::instance().
		insert(_ptr, collision_tag::EMonster, figure_type::ERect);

	auto sp_collision = wp_collision.lock();

	if (!sp_collision)return;

	this->bAttacking = false;
	sp_collision->bCollision = true;
	sp_collision->bCollisionSlideAnObject = true;
	sp_collision->bCollisionTargetPushFromForce = false;
	sp_collision->bHitEffect = true;
	sp_collision->bRender = true;
	sp_collision->bSlide = true;
	sp_collision->PushForce = 0.f;
	sp_collision->_size = { 80,80 };
	sp_collision->bSuperArmor = true;

	_Shadow.initialize();
	_Shadow._owner = (_ptr);
	_Shadow.correction = { 0,125 };
	_Shadow.bShadow = true;
	_Shadow.world_size_correction = { 90,85 };

	PaintSizeX = 220;
	PaintSizeY = 300;
	ScaleX = 1.f;
	ScaleY = 1.f;
	Speed = 200.f;
	Attack = { 90,100 };
	AttackStartDistance = 750.f;
	UniqueID = EObjUniqueID::BOSS;
	ObjectTag = Tag::monster;

	StateDuration = 3.f;

	auto HPBar = object_mgr::instance().insert_object<UIBossHPBar>();
	wp_UIBossHPBar = HPBar;
	sp_MyInfo = std::make_shared<BossInfo>();
	sp_MyInfo->CurrentHP = sp_MyInfo->MAXHP = 3000;
	HPBar->current = sp_MyInfo->CurrentHP;
	HPBar->goal_time = 1.f;
	HPBar->target = HPBar->max = sp_MyInfo->MAXHP;



	auto UI = object_mgr::instance().insert_object<UIBossName>(
		L"BOSS_NAMEBAR.bmp", L"BOSS_NAMEBAR");
	if (!UI)return;
	UI->_owner = _ptr;
};

void BOSS::render(HDC hdc, vec camera_pos, vec size_factor)
{
	object::render(hdc, camera_pos, size_factor);

	auto sp_Bmp = AnimDirSpriteUpdate();
	if (!sp_Bmp)return;

	// 공격 상태라면 여기서 공격 스프라이트로 교체해줘야함

	AnimUpdateFromCurrentState();
	_Shadow.render(hdc, camera_pos);

	vec DestPaintSize = { PaintSizeX * ScaleX, PaintSizeY * ScaleY };
	vec DestLoc = _transform->_location - camera_pos - (DestPaintSize * 0.5);

	GdiTransparentBlt(hdc,
		DestLoc.x, DestLoc.y
		, DestPaintSize.x, DestPaintSize.y
		, sp_Bmp->Get_MemDC(),
		CurrentColIdx * PaintSizeX,
		CurrentRowIdx * PaintSizeY,
		PaintSizeX, PaintSizeY,
		COLOR::MEGENTA());
}

Event BOSS::update(float dt)
{
	Event _Event = object::update(dt);

	StateDuration -= dt;
	CurrentHitCoolTime -= dt;

	UpdateDir();
	StateAction();
	StateTranslation();

	return _Event;
}

void BOSS::Hit(std::weak_ptr<object> _target)
{
	object::Hit(_target);

	auto sp_Target = _target.lock();
	if (!sp_Target)return;

	if (sp_Target->ObjectTag == object::Tag::player_attack && CurrentHitCoolTime < 0)
	{
		CurrentHitCoolTime = DefaultHitCoolTime;
		sound_mgr::instance().RandSoundKeyPlay("HIT_SOUND_NORMAL", { 1,2 }, 1);
		collision_mgr::instance().HitEffectPush(_transform->_location, 0.5f);


		// 공격 준비 , 공격 중에는 슈퍼아머 이므로 상태전이 하기전 체크
		if (CurrentState == EState::IDLE || CurrentState == EState::HIT)
		{
			StateDuration += 0.2f;
			CurrentState = BOSS::EState::HIT;
			HitCalc(sp_Target->Attack);
		}
		else
		{
			int Accuracy = math::Rand<int>({ 0,1 });
			if (Accuracy == 0)
				helper::MissMsg(_transform->_location);
			else
				HitCalc({ sp_Target->Attack.first * 0.5,sp_Target->Attack.second * 0.5 });
		}

		if (!sp_MyInfo)return;

		if (sp_MyInfo < 0)
		{
			StateDuration = 0.8f;
			CurrentState = EState::DIE;
			DieAction();
		}
	};
}

void BOSS::SetUp(std::weak_ptr<class object> AttackTarget, vec Location)
{
	if (!_transform)return;

	wp_AttackTarget = std::move(AttackTarget);
	_transform->_location = Location;
}

BOSS::~BOSS() noexcept
{
	DieAction();

}

// 공격중인 모션일때 로우인덱스 업데이트해주세요.
int BOSS::AnimAttackSpriteRowUpdateFromDir()
{
	
		math::EDir _EDir = math::checkDir(_transform->_dir);
		switch (_EDir)
		{
		case math::EDir::left:
			return 0;
			break;
		case math::EDir::right:
			return 2;
			break;
		case math::EDir::up:
			return 1;
			break;
		case math::EDir::down:
			return 3;
			break;
		default:
			break;
		}
	
		return 1;

}

// 호출전에 트랜스폼 방향 업데이트 먼저 해주세요.
std::shared_ptr<class Bmp> BOSS::AnimDirSpriteUpdate()
{
	return _transform->_dir.x < 0 ? sp_Bmps.first : sp_Bmps.second;
}

void BOSS::HitCalc(std::pair<int, int> AttackRange)
{
	float Atk = math::Rand<float>(AttackRange);
	vec RandVec = math::RandVec();
	RandVec.y = (abs(RandVec.y));
	vec MyLocation = _transform->_location;
	MyLocation.y -= 35;
	MyLocation.x += math::Rand<int>({ -40,+40 });

	object_mgr::instance().TextEffectMap[RGB(221, 221, 221)].
		push_back({ MyLocation ,vec{0,1}*3,
		1.f,int(Atk),std::to_wstring((int)Atk) });

	sp_MyInfo->CurrentHP -= Atk;

	if (sp_MyInfo->CurrentHP < 0)
	{
		CurrentState = EState::DIE;
		StateDuration = 0.8f;
	}

	auto sp_UIBossHPBar = wp_UIBossHPBar.lock();
	if (!sp_UIBossHPBar)return;
	sp_UIBossHPBar->target = sp_MyInfo->CurrentHP;
}

void BOSS::AttackStart()
{
	if (CurrentSKILL)CurrentSKILL();
}

void BOSS::AnimColUpdate()
{
	CurrentAnimDelta -= DeltaTime;
	if (CurrentAnimDelta < 0)
	{
		CurrentAnimDelta = AnimDelta;
		++CurrentColIdx;
		// 스프라이트에 따라서 테이블 달리 참조
		if (bAttackAnimSprite)
		{
			if (CurrentColIdx >= CurrentAttackAnimColNumTable[CurrentRowIdx])
				CurrentColIdx = 0;
		}
		else
		{
			if (CurrentColIdx >= CurrentAnimColNumTalble[CurrentRowIdx])
				CurrentColIdx = 0;
		}
	}
};

void BOSS::AnimUpdateFromCurrentState()
{

	switch (CurrentState)
	{
	case BOSS::EState::IDLE:
		_Shadow.CurrentShadowState = EShadowState::NORMAL;
		CurrentRowIdx = 0;
		CurrentColIdx = 0;
		break;
	case BOSS::EState::WALK:
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
		CurrentRowIdx = 1;
		AnimColUpdate();
		break;
	case BOSS::EState::BOXATTACK:
		_Shadow.CurrentShadowState = EShadowState::BIG;
		CurrentRowIdx = 2;
		AnimColUpdate();
		break;
	case BOSS::EState::JUMPATTACK:
		_Shadow.CurrentShadowState = EShadowState::BIG;
		CurrentRowIdx = 3;
		AnimColUpdate();
		break;
	case BOSS::EState::HIT:
		_Shadow.CurrentShadowState = EShadowState::BIG;
		CurrentRowIdx = 4;
		AnimColUpdate();
		break;
	case BOSS::EState::DIE:
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
		CurrentRowIdx = 5;
		AnimColUpdate();
		break;
	default:
		break;
	}
}


void BOSS::StateTranslation()
{
	if (StateDuration > 0)return;

	int DICE = math::Rand<int>({ 0,1 });

	/*switch (CurrentState)
	{
	case BOSS::EState::IDLE:
		StateSetUp(BOSS::EState::WALK, 1.5f);
		MoveMarkReNew();
		break;
	case BOSS::EState::WALK:
		StateSetUp(BOSS::EState::READY, 4);
		break;
	case BOSS::EState::READY:
		StateSetUp(BOSS::EState::CAST, 10);
		CastStart();
		break;
	case BOSS::EState::CAST:
		StateSetUp(BOSS::EState::IDLE, 3);
		break;
	case BOSS::EState::HIT:
		if (DICE == 1)
		{
			StateSetUp(BOSS::EState::WALK, 1.5);
		}
		else if (DICE == 2)
		{
			StateSetUp(BOSS::EState::IDLE, 0.5);
		}
		break;
	case BOSS::EState::DIE:
		bDie = true;
		break;
	default:
		break;
	}*/
}

void BOSS::StateAction()
{
	/*switch (CurrentState)
	{
	case BOSS::EState::IDLE:
		IdleAction();
		break;
	case BOSS::EState::WALK:
		WalkAction();
		break;
	case BOSS::EState::READY:
		break;
	case BOSS::EState::CAST:
		break;
	case BOSS::EState::HIT:
		break;
	default:
		break;
	}*/
}

void BOSS::UpdateDir()
{
	auto sp_AttackTarget = wp_AttackTarget.lock();
	if (!sp_AttackTarget)return;
	vec TargetLocation = sp_AttackTarget->_transform->_location;
	vec Dis = TargetLocation - _transform->_location;
	vec Dir = Dis.get_normalize();
	_transform->_dir = Dir;
}

void BOSS::StateSetUp(EState NewState, float Duration,float StateAnimDelta)
{
	CurrentState = NewState;
	StateDuration = Duration;
	BOSS::AnimDelta = StateAnimDelta;
	CurrentColIdx = 0;
}

void BOSS::ConditionCheck()
{

}

void BOSS::IdleAction()
{
	auto sp_AttackTarget = wp_AttackTarget.lock();
	if (!sp_AttackTarget)return;
	if (!_transform)return;
}

void BOSS::DieAction()
{
	auto sp_UIBOSSHPBar = wp_UIBossHPBar.lock();
	if (!sp_UIBOSSHPBar)return;
	sp_UIBOSSHPBar->bDie = true;

	auto sp_wp_UIBossName = wp_UIBossName.lock();
	if (!sp_wp_UIBossName)return;
	sp_wp_UIBossName->bDie = true;


}
