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
#include "BOSS_BOX.h"
#include "BOSS_SKILL.h"
#include "timer.h"
#include "BOSS_ROTBOX.h"
#include "BOSS_SKILL.h"
#include "Camera.h"



void BOSS::initialize()
{
	object::initialize();

	sp_Bmps.first = Bmp_mgr::instance().Find_Image_SP(L"BOSS_LEFT");
	sp_Bmps.second = Bmp_mgr::instance().Find_Image_SP(L"BOSS_RIGHT");
	sp_AttackBmp = Bmp_mgr::instance().Find_Image_SP(L"BOSS_ATTACK");
	sp_SOILBmp = Bmp_mgr::instance().Find_Image_SP(L"SOIL_EFFECT");


	wp_collision = collision_mgr::instance().
		insert(_ptr, collision_tag::EMonster, figure_type::ERect);

	wp_Attackcollision = collision_mgr::instance().insert(_ptr, collision_tag::EMonsterAttack, figure_type::ECircle);

	auto sp_Atkcomp = wp_Attackcollision.lock();
	if (!sp_Atkcomp) return;

	sp_Atkcomp->bCollision = false;
	sp_Atkcomp->bCollisionSlideAnObject = false;
	sp_Atkcomp->bCollisionTargetPushFromForce = true;
	sp_Atkcomp->bHitEffect = true;
	sp_Atkcomp->bRender = true;
	sp_Atkcomp->bSlide = true;
	sp_Atkcomp->PushForce = 10.f;
	sp_Atkcomp->_size = { 120,120};

	auto sp_collision = wp_collision.lock();

	if (!sp_collision)return;

	this->bAttacking = true;
	sp_collision->bCollision = true;
	sp_collision->bCollisionSlideAnObject = true;
	sp_collision->bCollisionTargetPushFromForce = false;
	sp_collision->bHitEffect = true;
	sp_collision->bRender = true;
	sp_collision->bSlide = true;
	sp_collision->PushForce = 0.f;
	sp_collision->_size = { 60,80 };
	sp_collision->bSuperArmor = true;

	_Shadow.initialize();
	_Shadow._owner = (_ptr);
	_Shadow.correction = { 0,125 };
	_Shadow.bShadow = true;
	_Shadow.world_size_correction = DefaultShadowWorldSizeCorrection;

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
	sp_MyInfo->CurrentHP = sp_MyInfo->MAXHP = 2000;
	HPBar->current = sp_MyInfo->CurrentHP;
	HPBar->goal_time = 1.f;
	HPBar->target = HPBar->max = sp_MyInfo->MAXHP;



	auto UI = object_mgr::instance().insert_object<UIBossName>(
		L"BOSS_NAMEBAR.bmp", L"BOSS_NAMEBAR");
	if (!UI)return;
	UI->_owner = _ptr;
	wp_UIBossName = UI;

	PatternTableNum = PatternTable.size();
	CurrentPatternIdx = 0;
	
	SOILs.resize(36 * 3);

	
	PatternMap[EPattern::BOXATTACK] = std::bind(&BOSS::BoxAttackStart, this);
	PatternMap[EPattern::ROTBOXATTACK] = std::bind(&BOSS::RotationBoxAttackStart, this);
	PatternMap[EPattern::BOXDIRECTPILLARATTACK] = std::bind(&BOSS::BoxDirectPillarAttackStart, this);
	PatternMap[EPattern::PILLARMULTIPLEATTACK] = std::bind(&BOSS::PillarMultipleAttackStart, this);
	PatternMap[EPattern::PILLARSPIRALATTACK] = std::bind(&BOSS::PillarSpiralAttackStart, this);
};

void BOSS::render(HDC hdc, vec camera_pos, vec size_factor)
{
	object::render(hdc, camera_pos, size_factor);

	// 여기서 스프라이트 이미지 판단
	auto sp_Bmp = AnimDirSpriteUpdate();
	if (!sp_Bmp)return;

	// 여기서 캐릭터 방향으로 Row Idx 판단
	AnimUpdateFromCurrentState();
	_Shadow.render(hdc, camera_pos);

	vec DestPaintSize = { PaintSizeX * ScaleX, PaintSizeY * ScaleY };
	vec DestLoc = _transform->_location - camera_pos - (DestPaintSize * 0.5);
	DestLoc += RenderCorrection;

	// Col 인덱스 업데이트.
	AnimColUpdate();

	GdiTransparentBlt(hdc,
		DestLoc.x, DestLoc.y
		, DestPaintSize.x, DestPaintSize.y
		, sp_Bmp->Get_MemDC(),
		CurrentColIdx * PaintSizeX,
		CurrentRowIdx * PaintSizeY,
		PaintSizeX, PaintSizeY,
		COLOR::MRGENTA());

	SOIL_EffectRender(hdc, camera_pos);
}

Event BOSS::update(float dt)
{
	Event _Event = object::update(dt);

	StateDuration -= dt;
	CurrentHitCoolTime -= dt;
	SoilEffectDuration -= dt;

	UpdateDir();
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
		if (CurrentState == EState::IDLE || CurrentState == EState::HIT ||
			CurrentState== EState::Taunt)
		{
			CurrentState = EState::HIT;
			StateDuration += 0.1f;
			AnimDelta = 0.1f;
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

		if (sp_MyInfo->CurrentHP < 0)
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
void BOSS::JumpStart()
{
	CurrentJumpAcceleration = 0;
	_Shadow.CurrentShadowState = EShadowState::BIG;

	/// <summary>
	/// 정해진 시간동안 그림자 크기를 키우고 낙하한다고 정한 시간동안 크기를 줄여나감
	/// </summary>

	Timer::instance().event_regist(time_event::ERemaingWhile, 0.5, [this] {
		RenderCorrection += vec{ 0 ,    -1 }  *3000 * DeltaTime; 

		_Shadow.world_size_correction.first += 1;
		_Shadow.world_size_correction.second+= 1;
		return true; });

	Timer::instance().event_regist(time_event::EOnce, 0.5, [this] {

		Timer::instance().event_regist(time_event::ERemaingWhile, 0.5, [this] {
			RenderCorrection += vec{ 0 ,    1 }  *3000 * DeltaTime;

			_Shadow.CurrentShadowState = EShadowState::NORMAL;
			_Shadow.world_size_correction.first -= 1;
			_Shadow.world_size_correction.second -= 1;
			return true; });

		return true; });


	Timer::instance().event_regist(time_event::EOnce, 1.5001, [this] {
		RenderCorrection = { 0,0 };
		return true;
		});
}
void BOSS::JumpTracking()
{
	if (!_transform)return;
	
	float Speed = JumpInitTrackSpeed + CurrentJumpAcceleration;
	if (TargetDistance >50)
	_transform->_location += _transform->_dir *DeltaTime * Speed;
	CurrentJumpAcceleration += JumpAcceleration * DeltaTime;
}
void BOSS::JumpEndAnimPlay()
{
	CurrentJumpAcceleration = 0;
	CurrentRowIdx = 3;
	CurrentColIdx = 2;
	bAttackAnimSprite = false;
	bAnimLoop = false;
	AnimDelta = 0.08f;
	CurrentAnimColMax = 7;
}


void BOSS::JumpEnd()
{
	_Shadow.CurrentShadowState = EShadowState::NORMAL;
	_Shadow.world_size_correction = DefaultShadowWorldSizeCorrection;
}
void BOSS::AttackAnimOn()
{
	bAttackAnimSprite = true;
	CurrentRowIdx = CalcAttackAnimRowIdxFromDir();
	CurrentColIdx = 0; 
}

void BOSS::AttackAnimEnd()
{
	bAttackAnimSprite = false;
}

void BOSS::SOIL_EffectSetUp()
{
	SoilEffectDuration = 0.6f;

	float InitRadius = 50.f;
	vec InitLocation = _transform->_location;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 36; ++j)
		{
			SOIL _SOIL;

			vec PaintLocation  = InitLocation + (math::dir_from_angle(j * 10) * InitRadius);
			int Row = math::Rand<int>({0,6});
			int Col = math::Rand<int>({ 0,3 });
			_SOIL.X = PaintLocation.x;
			_SOIL.Y = PaintLocation.y;
			_SOIL.Row = Row;
			_SOIL.Col = Col;
			SOILs[i * 36 + j] = std::move(_SOIL);
		}
		InitRadius += 70;
	}
}
void BOSS::SOIL_EffectRender(HDC hdc,vec camera_pos)
{
	if (SoilEffectDuration < 0)return;
	if (!sp_SOILBmp)return;

	SoilEffectRenderTick += DeltaTime;

	if (SoilEffectRenderTick > 0.1f)
	{
		SoilEffectRenderTick = 0;
		SoilEffectRenderRowAdd--;
	}

	int AnimRowAdd = 0;

	float  AddY = 20;

	float x = (SoilEffectDuration)*math::PI / 0.5f;
	float c = std::cosf(x);

	AddY *= c;

	for (auto& _SOIL : SOILs)
	{
		int X = _SOIL.X - camera_pos.x - 30;
		int Y = _SOIL.Y - camera_pos.y - 30 + AddY;

		GdiTransparentBlt(hdc, X, Y, 60, 60, sp_SOILBmp->Get_MemDC(),
			_SOIL.Col * 60, (_SOIL.Row+ SoilEffectRenderRowAdd) * 60, 60, 60, COLOR::MRGENTA());
	};
}
void BOSS::SOIL_EffectEnd()
{
	SoilEffectRenderRowAdd = 0;
	SoilEffectRenderTick = 0;
}
/// <summary>
/// 공격 방향으로 공격 애니메이션 스프라이트 기준 Row 인덱스 반환
/// 함수 호출전 방향 갱신이 필요함.
/// </summary>
/// <returns></returns>
int BOSS::CalcAttackAnimRowIdxFromDir()
{
	math::EDir _EDir = math::checkDir(_transform->_dir);
	int AttackSpriteRowIdx = 0;
	switch (_EDir)
	{
	case math::EDir::left:
		AttackSpriteRowIdx = 0;
		break;
	case math::EDir::right:
		AttackSpriteRowIdx = 2; 
		break;
	case math::EDir::up:
		AttackSpriteRowIdx = 3;
		break;
	case math::EDir::down:
		AttackSpriteRowIdx = 1;  
		break;
	default:
		break;
	}
	return AttackSpriteRowIdx;
}

// 호출전에 트랜스폼 방향 업데이트 먼저 해주세요.
std::shared_ptr<class Bmp> BOSS::AnimDirSpriteUpdate()
{
	if (bAttackAnimSprite)
	{
		return sp_AttackBmp;
	}
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
		if (CurrentAnimColMax < CurrentColIdx)
		{
			if (bAnimLoop)
			{
				CurrentColIdx = 0;
			}
			else
			{
				CurrentColIdx = CurrentAnimColMax;  
			}
		}
	}
}

void BOSS::AnimUpdateFromCurrentState()
{
	if (bAttackAnimSprite)
	{
		// 공격 할때는 공격 시작시 방향으로 Row 고정
		return;
	}
	switch (CurrentState)
	{
	case BOSS::EState::IDLE:
		CurrentColIdx = 0;
		CurrentRowIdx = 0;
		break;
	case BOSS::EState::Taunt:
		CurrentRowIdx = 1;
		break;
	case BOSS::EState::ATTACK:
		break;
	case BOSS::EState::HIT:
		CurrentRowIdx = 4;
		break;
	case BOSS::EState::DIE:
		CurrentRowIdx = 5;  
		break;
	default:
		break;
	}
}

void BOSS::StateTranslation()
{
	if (StateDuration > 0)return;

	switch (CurrentState)
	{
	case BOSS::EState::IDLE:
 		StateSetUp(EState::Taunt, math::Rand<float>({ 0,2 }), 0.1f,0,true,8);
		break;
	case BOSS::EState::Taunt:
		CurrentSKILL = PatternMap[PatternTable[CurrentPatternIdx]];
		if (CurrentSKILL)CurrentSKILL();

		if (++CurrentPatternIdx >= PatternTableNum)
			CurrentPatternIdx = 0;
		break;
	case BOSS::EState::ATTACK:
		StateSetUp(EState::IDLE, 1.5f, 1.5f,0,false,0);
		break;
	case BOSS::EState::HIT:
		StateSetUp(EState::Taunt, 0.1f, 0.1f,0,true,8);
		break;
	case BOSS::EState::DIE:
		bDie = true;
		break;
	default:
		break;
	}
}

void BOSS::UpdateDir()
{
	auto sp_AttackTarget = wp_AttackTarget.lock();
	if (!sp_AttackTarget)return;
	vec TargetLocation = sp_AttackTarget->_transform->_location;
	vec Dis = TargetLocation - _transform->_location;
	TargetDistance = Dis.length();
	vec Dir = Dis.get_normalize();
	_transform->_dir = Dir;
}

void BOSS::StateSetUp(EState NewState, float Duration,float StateAnimDelta,int SetStartColIdx,
	bool SetAnimLoop,int AnimColMax)
{
	CurrentState = NewState;
	StateDuration = Duration;
	BOSS::AnimDelta = StateAnimDelta;
	CurrentAnimDelta = StateAnimDelta; 
	CurrentColIdx = SetStartColIdx;
	bAnimLoop = SetAnimLoop; 
	CurrentAnimColMax = AnimColMax; 
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


void BOSS::BoxAttack()
{
	vec Loc = _transform->_location;
	float Distance = 220;
	float Duration = 1.5;
	float DurationIncrement = 0.8f;

	std::vector<vec> BoxLocTable{
		Loc + vec{ 0 , 1 }  *Distance,
		Loc + vec{ 1 , 0 }  *Distance,
		Loc + vec{ -1 ,  0 } *Distance
	}; 

	for (const auto& BoxSpwanLocation : BoxLocTable)
	{
		object_mgr::instance().insert_object<BOSS_BOX>
		(BoxSpwanLocation, Duration, wp_AttackTarget);

		Timer::instance().event_regist(time_event::EOnce, Duration, GetAttackAnimPakage());

		Duration += DurationIncrement;
	}
}

void BOSS::RotationBoxAttack(size_t NUM)
{

	float AngleIncrement = 360 / NUM;
	float Angle = 0.01;
	float RotationDurationInit = 5.f;
	float RotaionDurationIncrement = 0.8f;
	float Distance = 300;
	float Duration = 1.5;
	vec Loc = _transform->_location;
	Loc.y += 50;

	float DurationIncrement = 0.8f;


	for (int i = 0; i < NUM; ++i)
	{
		vec Dir = math::dir_from_angle(Angle) * Distance;
		object_mgr::instance().insert_object<BOSS_ROTBOX>
		(Loc+Dir, Duration, wp_AttackTarget, _transform->_location, RotationDurationInit);
		Duration += DurationIncrement;
		RotationDurationInit += RotaionDurationIncrement;
		Angle += AngleIncrement;

		Timer::instance().event_regist(time_event::EOnce, RotationDurationInit- DurationIncrement, GetAttackAnimPakage());
	}
}

void BOSS::PillarCircle()
{
	int AngleNum = 14;
	vec MyLocation = _transform->_location + vec{ -50,50 };
	float InitAngle = 0;
	float InitRadius = 200;
	float AngleIncrement = 360 / AngleNum;

	for (int j = 0; j < AngleNum; ++j)
	{
		vec InitLocation = MyLocation + math::dir_from_angle(InitAngle) * InitRadius;
		object_mgr::instance().insert_object<BOSS_SKILL>(InitLocation);

		InitAngle += AngleIncrement;
	}
}

void BOSS::PillarMultipleAttack()
{
	Timer::instance().event_regist_ReWhileDelta(3, 0.25f, &BOSS::PillarPredictionAttack, this);
}

void BOSS::PillarSpiralAttack()
{
	int CircleNum = 7;
	int AngleNum = 12;

	float RadiusIncrement = 200;
	std::shared_ptr<float> InitRadius = std::make_shared<float>(200);
	std::shared_ptr<float> InitAngle = std::make_shared<float>(0);
	std::shared_ptr<int> i = std::make_shared<int>(0);

	float AngleIncrement = 360 / AngleNum;

	vec MyLocation = _transform->_location + vec{ 50,100 };

	Timer& _Timer = Timer::instance();

	_Timer.event_regist_ReWhileDelta(2.5, 2.5 / CircleNum, [=]() {
		if (!InitRadius)return;
		if (!InitAngle)return;
		if (!i)return;

		for (int j = 0; j < AngleNum; ++j)
		{
			vec InitLocation = MyLocation + math::dir_from_angle(*InitAngle) * *InitRadius;
			object_mgr::instance().insert_object<BOSS_SKILL>(InitLocation);

			*InitAngle += AngleIncrement;
		}
		*InitRadius += RadiusIncrement;
		if (*i % 2 == 1) {
			*InitAngle = 0;
		}
		else *InitAngle = AngleIncrement / 2;

		(*i)++;
		});


}

void BOSS::BoxDirectPillarAttack()
{
	vec Loc = _transform->_location + vec{ 30,100 };

	int AttackCount = 6;
	float Distance = 220;
	float Duration = 0.7;
	float DurationIncrement = 0.5f;
	float AngleIncrement = 360 / 6;

	vec InitDir = vec{ 1,0 };

	for (int i = 0; i < AttackCount; ++i)
	{
		auto BOX = object_mgr::instance().insert_object<BOSS_BOX>
			(Loc + InitDir * Distance, Duration, wp_AttackTarget);

		Timer::instance().event_regist(time_event::EOnce, Duration, GetAttackAnimPakage());

		BOX->FlyTimeInAddY = 30;
		BOX->bLaunchPillar = true;
		Duration += DurationIncrement;
		InitDir = math::rotation_dir_to_add_angle(InitDir, AngleIncrement);
	}
}

void BOSS::BoxAttackStart()
{
	CurrentRowIdx = 2;
	bAttackAnimSprite = false;
	bAnimLoop = false;
	StateSetUp(BOSS::EState::ATTACK, 5.f, 0.2f, 0,false,1);
	BoxAttack();
	// 
	Timer::instance().event_regist(time_event::EOnce, 5.f, [this]() {
		bAttackAnimSprite = false; 
		return true; 
		});
}

void BOSS::RotationBoxAttackStart()
{
	float Duration = 10.f; 

	CurrentRowIdx = 2;
	bAttackAnimSprite = false;
	bAnimLoop = false;
	StateSetUp(BOSS::EState::ATTACK, Duration, 0.2f, 0, false, 1);
	RotationBoxAttack(6);
	// 

	Timer::instance().event_regist(time_event::EOnce, Duration-0.3f, [this]() {
		bAttackAnimSprite = false;
		return true;
		});
}

void BOSS::BoxDirectPillarAttackStart()
{
	float Duration = 7;
	CurrentRowIdx = 2;
	bAttackAnimSprite = false;
	bAnimLoop = false;
	StateSetUp(BOSS::EState::ATTACK, Duration, 0.2f, 0, false, 1);
	BoxDirectPillarAttack();
	// 

	Timer::instance().event_regist(time_event::EOnce, Duration, [this]() {
		bAttackAnimSprite = false;
		return true;
		});
};

void BOSS::PillarMultipleAttackStart()
{
	float Duration = 7;
	CurrentRowIdx = 2;
	bAttackAnimSprite = false;
	StateSetUp(BOSS::EState::ATTACK, Duration, 0.2f, 0, true, 1);
	PillarMultipleAttack();

	Timer::instance().event_regist(time_event::EOnce, 0.2f * 12, [this]() {
		bAnimLoop= false;
		return true;
		});
}

void BOSS::PillarSpiralAttackStart()
{
	float Duration = 10.f;
	CurrentRowIdx = 3;
	bAttackAnimSprite = false;
	StateSetUp(BOSS::EState::ATTACK, Duration, 0.25f, 0, false, 1);

	Timer::instance().event_regist(time_event::EOnce, 0.4,
		[this]() {JumpStart();  return true; });

	Timer::instance().event_regist(time_event::ERemaingWhile, 1.3,
		[this]() {JumpTracking(); return true; });

	Timer::instance().event_regist(time_event::EOnce, 1.2, [this]  ()  { 
		JumpEndAnimPlay(); return true;  });

	Timer::instance().event_regist(time_event::EOnce, 1.3,
		[this]() {SOIL_EffectSetUp(); JumpEnd();  PillarSpiralAttack(); 
	ShockAttackStart();
	auto sp_camera = object_mgr::instance()._Camera.lock();
	if (!sp_camera)return false ;
	sp_camera->camera_shake(75, vec{ 0,-1 }, 2);
	return true; });

	Timer::instance().event_regist(time_event::EOnce, 2,
		[this]() {ShockAttackEnd(); return true; });
}


void BOSS::AttackAnimPlay()
{
	//if (!_transform)return;
	//vec MyLocation = _transform->_location;
	//vec TargetLocation = auto TargetLocation = wp_AttackTarget.lock();
	int RowIdx = CalcAttackAnimRowIdxFromDir();
	CurrentRowIdx = RowIdx;
	bAttackAnimSprite = true;
	bAnimLoop = false;
	CurrentAnimColMax = 5;
	BOSS::AnimDelta = 0.1f;
	CurrentAnimDelta = AnimDelta;
	CurrentColIdx = 0;
}

void BOSS::PillarPredictionAttack()
{
	auto sp_Target = wp_AttackTarget.lock();
	if (!sp_Target)return;

	float PredictionTargetSpeed = 400.f;

	int PillarBeetWeenDistance = 150;
	vec MyLocation = _transform->_location;
	vec TargetLocation = sp_Target->_transform->_location;
	vec TargetDir = sp_Target->_transform->_dir;
	
	vec PredictionTargetLocation = TargetLocation+ TargetDir* PredictionTargetSpeed; 

	vec PreDis = PredictionTargetLocation - MyLocation;
	vec PreDir = PreDis.get_normalize();


	auto Inner = std::make_shared<int>(0);

	Timer::instance().event_regist_ReWhileDelta(2, 0.20f, [=]() {
		if (!Inner)return;

		vec InitLocation = MyLocation + PreDir * PillarBeetWeenDistance * (*Inner + 1);
		object_mgr::instance().insert_object<BOSS_SKILL>(InitLocation);
		(*Inner)++;
		});
}

void BOSS::PillarDirectAttack()
{
	auto sp_Target = wp_AttackTarget.lock();
	if (!sp_Target)return;

	int PillarBeetWeenDistance = 150;
	float PillarSpawnCycle = 0.2f;
	vec MyLocation  =_transform->_location; 
	vec Dis  = sp_Target->_transform->_location - _transform->_location;
	vec Dir = Dis.get_normalize();

	auto Inner = std::make_shared<int>(0);

	Timer::instance().event_regist_ReWhileDelta(1.5f, 0.14f, [=]() {
		if (!Inner)return;

		vec InitLocation = MyLocation + Dir * PillarBeetWeenDistance * (*Inner + 1);
		object_mgr::instance().insert_object<BOSS_SKILL>(InitLocation);
		(*Inner)++;
		});
}

void BOSS::ShockAttackStart()
{
	auto sp_Atkcomp = wp_Attackcollision.lock();
	if (!sp_Atkcomp) return;
	sp_Atkcomp->bCollision = true;
	bAttacking = true;
	
}

void BOSS::ShockAttackEnd()
{
	auto sp_Atkcomp = wp_Attackcollision.lock();
	if (!sp_Atkcomp) return;
	sp_Atkcomp->bCollision = false;
	bAttacking = false;

}

