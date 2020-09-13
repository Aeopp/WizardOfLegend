#include "pch.h"
#include "Monster.h"

#include "WizardBall.h"
#include "ICE_Crystal.h"
#include "UIBossHPBar.h"
#include "MIDDLE_BOSS.h"
#include "WizardFire.h"
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

#include "BossCrystal.h"

#include "UIBossName.h"
#include "ARCHER.h"
#include "SwordMan.h"
#include "WIZARD.h"
#include "HomingBlast.h"
#include "BossFireDragon.h"
#include "RotationBoomerang.h"
#include "BossBoomerang.h"
#include "Input_mgr.h"
#include "Monster.h"
#include "Boss_Blast.h"


MIDDLE_BOSS::~MIDDLE_BOSS() noexcept
{
	DieAction();
}

void MIDDLE_BOSS::initialize()
{
	object::initialize();

	sp_Bmps.first = Bmp_mgr::instance().Find_Image_SP(L"MIDDLE_BOSS_LEFT");
	sp_Bmps.second = Bmp_mgr::instance().Find_Image_SP(L"MIDDLE_BOSS_RIGHT");

	FireImg = Bmp_mgr::instance().Find_Image_SP(L"WIZARD_FIRE");

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
	sp_collision->_size = { 40,80 };
	sp_collision->bSuperArmor = true;

	_Shadow.initialize();
	_Shadow._owner = (_ptr);
	_Shadow.correction = { 0,125 };
	_Shadow.bShadow = true;
	_Shadow.world_size_correction={ 90,85};

	PaintSizeX = 220;
	PaintSizeY = 300;
	ScaleX = 1.f;
	ScaleY = 1.f;
	Speed = 200.f;
	Attack = { 90,100 };
	AttackStartDistance = 750.f;
	UniqueID = EObjUniqueID::MIDDLE_BOSS;
	ObjectTag = Tag::monster;

	StateDuration = 3.f;

	auto HPBar = object_mgr::instance().insert_object<UIBossHPBar>();
	wp_UIBossHPBar = HPBar;
	sp_MyInfo = std::make_shared<BossInfo>();
	sp_MyInfo->CurrentHP = sp_MyInfo->MAXHP = 100;
	HPBar->current = sp_MyInfo->CurrentHP;
	HPBar->goal_time = 1.f;
	HPBar->target = HPBar->max = sp_MyInfo->MAXHP;

	auto UI = object_mgr::instance().insert_object<UIBossName>(
	L"MIDDLEBOSS_NAMEBAR.bmp",L"MIDDLEBOSS_NAMEBAR");
	if (!UI)return;
	UI->_owner = _ptr;
	wp_UIBossName = UI;

	PatternTableNum = PatternTable.size();
	CurrentPatternIdx = 0;
	render_size = { 300,300 };
	
	_Burning_Info.particle_range.y = _Burning_Info.particle_range.y * 2;
	_Burning_Info.render_world_size_range *= 2;
};

void MIDDLE_BOSS::render(HDC hdc, vec camera_pos, vec size_factor)
{
	object::render(hdc, camera_pos, size_factor);

	Freezing_render(hdc, _transform->_location - camera_pos);
	
	if ( ! _Freezing_Info.IsFreezing()  ) 
	{
		auto sp_Bmp = AnimDirSpriteUpdate();
		if (!sp_Bmp)return;

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
			COLOR::MRGENTA());


		Burning_render(hdc, _transform->_location - camera_pos);
	}
}

Event MIDDLE_BOSS::update(float dt)
{
	Event _Event = object::update(dt);

	StateDuration -= dt;
	CurrentHitCoolTime -= dt;
	MonsterSpawnCurrentTick -= dt;

	Freezing_update(dt, wp_collision);
	burning_update(dt);
	

	if (_Freezing_Info.IsFreezing())return Event::None;
	
	UpdateDir();
	StateAction();
	StateTranslation();

	return _Event;
}

void MIDDLE_BOSS::Hit(std::weak_ptr<object> _target)
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
		if ( CurrentState ==  EState::IDLE || CurrentState == EState::HIT)
		{
			StateDuration += 0.2f;
			CurrentState = MIDDLE_BOSS::EState::HIT;
			HitCalc(sp_Target->Attack);
		}
		else
		{
			int Accuracy =math::Rand<int>({ 0,1 });
			if (Accuracy == 0)
				helper::MissMsg(_transform->_location);
			else
				HitCalc({sp_Target->Attack.first*0.5,sp_Target->Attack.second*0.5});
		}

		if (!sp_MyInfo)return;
		if (sp_MyInfo->CurrentHP < 0 && !_Freezing_Info.IsFreezing())
		{
			CurrentState = EState::DIE;
			StateDuration = 0.8f;
			DieAction(); 
		}
	};




}

void MIDDLE_BOSS::SetUp(std::weak_ptr<class object> AttackTarget, vec Location)
{
	if (!_transform)return;

	wp_AttackTarget = std::move(AttackTarget); 
	_transform->_location = Location;
}

std::shared_ptr<class Bmp> MIDDLE_BOSS::AnimDirSpriteUpdate()
{
	return _transform->_dir.x < 0 ? sp_Bmps.first : sp_Bmps.second;
}

void MIDDLE_BOSS::HitCalc(std::pair<int,int> AttackRange)
{
	float Atk = math::Rand<float>(AttackRange);
	COLORREF _HitEffectColor = RGB(221, 221, 221);
	std::wstring Msg = std::to_wstring((int)Atk);


	if (_Freezing_Info.IsFreezing())
	{
		Atk *= Freezing_Interface::Amplification_factor;
		_HitEffectColor = Freezing_Interface::EffectColor;
		Msg += L"!";
	}
	else if (_Burning_Info.IsBurning())
	{
		Atk *= Burning_Interface::Amplification_factor;
		_HitEffectColor = Burning_Interface::EffectColor;
		Msg += L"!";
	}


	vec RandVec = math::RandVec();
	RandVec.y = (abs(RandVec.y));
	vec MyLocation = _transform->_location;
	MyLocation.y -= 35;
	MyLocation.x += math::Rand<int>({ -40,+40 });

	object_mgr::instance().TextEffectMap[_HitEffectColor].
		push_back({ MyLocation ,vec{0,1}*3,
		1.f,int(Atk),std::move(Msg) });

	sp_MyInfo->CurrentHP -= Atk;
	Monster::MonsterHitPlayerSignatureGageAdd(Atk);
	if (sp_MyInfo->CurrentHP < 0)
	{
		CurrentState = EState::DIE;
		StateDuration = 0.8f;
	}

	auto sp_UIBossHPBar = wp_UIBossHPBar.lock();
	if (!sp_UIBossHPBar)return;
	sp_UIBossHPBar->target = sp_MyInfo->CurrentHP;
}

void MIDDLE_BOSS::ReadyAction()
{
	SummonMonster();
}

void MIDDLE_BOSS::CastAction()
{
	SummonMonster();
}

void MIDDLE_BOSS::MoveMarkReNew()
{
	if (!_transform)return;
	MoveMark = math::RandVec();
}

void MIDDLE_BOSS::WalkAction()
{
	if (!_transform)return;
	_transform->_location += MoveMark * Speed * DeltaTime;
}

void MIDDLE_BOSS::AttackStart()
{
	EPattern SelectPattern =PatternTable[CurrentPatternIdx];

	switch (SelectPattern)
	{
	case MIDDLE_BOSS::EPattern::FIRE:
		CurrentSKILL = std::bind(&MIDDLE_BOSS::BOSS_Skill_Fire, this);
		break;
	case MIDDLE_BOSS::EPattern::BLAST:
		CurrentSKILL = std::bind(&MIDDLE_BOSS::BOSS_SKill_Blast, this);
		break;
	case MIDDLE_BOSS::EPattern::CRYSTAL:
		CurrentSKILL = std::bind(&MIDDLE_BOSS::BOSS_Skill_ICECrystal, this, 3);
		break;
	case MIDDLE_BOSS::EPattern::BOOMERANG:
		CurrentSKILL = std::bind(&MIDDLE_BOSS::BOSS_SKill_Boomerang, this,3);
		break;
	case MIDDLE_BOSS::EPattern::FIRE_DRAGON:
		CurrentSKILL = std::bind(&MIDDLE_BOSS::BOSS_SKill_FireDragon, this ,  6);
		break;
	default:
		break;
	}		

	// TODO TEST 
	CurrentPatternIdx++;
	CurrentPatternIdx %= PatternTableNum;

	if (CurrentSKILL)CurrentSKILL();
}

void MIDDLE_BOSS::AnimColUpdate()
{
	CurrentAnimDelta -= DeltaTime;
	if (CurrentAnimDelta < 0)
	{
		CurrentAnimDelta = AnimDelta;
		++CurrentColIdx;
		if (CurrentColIdx >= CurrentAnimColNumTalble[CurrentRowIdx])
		{
			CurrentColIdx = 0;
		}
	}
};

void MIDDLE_BOSS::AnimUpdateFromCurrentState()
{

	switch (CurrentState)
	{
	case MIDDLE_BOSS::EState::IDLE:
		_Shadow.CurrentShadowState = EShadowState::NORMAL;
		CurrentRowIdx = 0;
		CurrentColIdx = 0; 
		break;
	case MIDDLE_BOSS::EState::WALK:
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
		CurrentRowIdx = 1;
		AnimColUpdate();
		break; 
	case MIDDLE_BOSS::EState::READY:
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
		CurrentRowIdx = 2;
		AnimColUpdate();
		break; 
	case MIDDLE_BOSS::EState::CAST:
		_Shadow.CurrentShadowState = EShadowState::BIG;
		CurrentRowIdx = 3;
		AnimColUpdate();
		break;
	case MIDDLE_BOSS::EState::HIT:
		_Shadow.CurrentShadowState = EShadowState::BIG;
		CurrentRowIdx = 4; 
		AnimColUpdate();
		break;
	case MIDDLE_BOSS::EState::DIE:
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
		CurrentRowIdx = 5;
		AnimColUpdate();
		break;
	default:
		break;
	}
}

void MIDDLE_BOSS::StateTranslation()
{
	if (StateDuration > 0)return;

	int DICE = math::Rand<int>({ 0,1 });

	switch (CurrentState)
	{
	case MIDDLE_BOSS::EState::IDLE:
		StateSetUp(MIDDLE_BOSS::EState::WALK, 1.5f);
		MoveMarkReNew();
		break;
	case MIDDLE_BOSS::EState::WALK:
		StateSetUp(MIDDLE_BOSS::EState::READY, 4);
		break;
	case MIDDLE_BOSS::EState::READY:
		StateSetUp(MIDDLE_BOSS::EState::CAST, 10);
		AttackStart();
		break;
	case MIDDLE_BOSS::EState::CAST:
		StateSetUp(MIDDLE_BOSS::EState::IDLE, 3);
		break;
	case MIDDLE_BOSS::EState::HIT:
		if (DICE == 1)
		{
			StateSetUp(MIDDLE_BOSS::EState::WALK, 1.5);
		}
		else if (DICE == 2)
		{
			StateSetUp(MIDDLE_BOSS::EState::IDLE, 0.5);
		}
		break;
	case MIDDLE_BOSS::EState::DIE:
		bDie = true;
		break;
	default:
		break;
	}
}

void MIDDLE_BOSS::StateAction()
{
	switch (CurrentState)
	{
	case MIDDLE_BOSS::EState::IDLE:
		IdleAction();
		break;
	case MIDDLE_BOSS::EState::WALK:
		WalkAction();
		break;
	case MIDDLE_BOSS::EState::READY:
		ReadyAction();
		break;
	case MIDDLE_BOSS::EState::CAST:
		CastAction();
		break;
	case MIDDLE_BOSS::EState::HIT:
		break;
	default:
		break;
	}
}

void MIDDLE_BOSS::UpdateDir()
{
	auto sp_AttackTarget = wp_AttackTarget.lock();
	if (!sp_AttackTarget)return;
	vec TargetLocation = sp_AttackTarget->_transform->_location;
	vec Dis = TargetLocation - _transform->_location;
	vec Dir = Dis.get_normalize();
	_transform->_dir = Dir;
}

void MIDDLE_BOSS::StateSetUp(EState NewState, float Duration)
{
	CurrentState = NewState;
	StateDuration = Duration;
	CurrentColIdx = 0;
}

void MIDDLE_BOSS::ConditionCheck()
{

}

void MIDDLE_BOSS::IdleAction()
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

void MIDDLE_BOSS::BOSS_Skill_Fire()
{
	Timer::instance().event_regist_ReWhileDelta(8.f, 0.1f, &MIDDLE_BOSS::FireSpawn, this);
}

void MIDDLE_BOSS::BOSS_SKill_BallSpawn()
{
	auto sp_Target = wp_AttackTarget.lock();
	if (!sp_Target)return;
	TargetLocation = sp_Target->_transform->_location;

	Timer::instance().event_regist_ReWhileDelta(2.f, 2.f/7,&MIDDLE_BOSS::BallSpawn,this,7);
}

void MIDDLE_BOSS::BOSS_SKill_HomingBlast()
{
	BlastSpawn(4);
}

void MIDDLE_BOSS::FireSpawn()
{
	if (!_transform)return;
	auto NormalAttack = object_mgr::instance().insert_object<WizardFire>();
	if (!NormalAttack)return;


	NormalAttack->_owner = _owner;
	NormalAttack->wp_Target = wp_AttackTarget;
	NormalAttack->speed = 1000.f;
	auto sp_Target = wp_AttackTarget.lock();
	if (!sp_Target)return;

	
	vec Dis = sp_Target->_transform->_location - _transform->_location;
	vec Dir = Dis.get_normalize();
	vec SpawnLocation = _transform->_location + Dir * 15.f;

	NormalAttack->Cast(SpawnLocation, Dis.get_normalize(), 0, FireImg);
}

void MIDDLE_BOSS::BallSpawn(size_t NUM)
{
	vec InitDir = math::dir_from_angle(BallSpawnSkillCurrentAngle);
	vec InitPos = TargetLocation+InitDir * 500.f;

	auto sp_WizBall = object_mgr::instance().insert_object<WizardBall>(wp_AttackTarget , InitPos );
	if (!sp_WizBall)return;

	sp_WizBall->HP = 50;
	BallSpawnSkillCurrentAngle += 360.f/NUM;
}

void MIDDLE_BOSS::BOSS_Skill_ICECrystal(size_t NUM)
{
	object_mgr& _object_mgr = object_mgr::instance();

	vec OuterTopRight = vec::unit_diagonal_vec();
	vec InnerTopRight = OuterTopRight;

	float DistanceFromBoss = 250;
	float DistanceFromRotationCenter = 250;
	float CrystalBeetwenAngle = (360 / NUM)-1;
	float RotationDurationMin= 8.f;
	float RotationDurationAdd = 0.2f;
	float AttackSpeed = 2000.f;

	float RotationDuration = RotationDurationMin;

	for (int i = 0; i < NUM; ++i)
	{
		for (int j = 0; j < NUM; ++j)
		{
			vec RotationCenter = _transform->_location +
				(OuterTopRight * DistanceFromBoss) + (InnerTopRight * DistanceFromRotationCenter);

			auto _Ice = _object_mgr.insert_object<BossCrystal>
			(RotationCenter,RotationDuration,wp_AttackTarget,AttackSpeed);
			if (!_Ice)return;

			_Ice->_transform->_dir = InnerTopRight;

			InnerTopRight= math::rotation_dir_to_add_angle(InnerTopRight, CrystalBeetwenAngle);
			RotationDuration += RotationDurationAdd;
		}
		OuterTopRight = math::rotation_dir_to_add_angle(OuterTopRight, CrystalBeetwenAngle);
	};
}

void MIDDLE_BOSS::BOSS_SKill_FireDragon(size_t NUM)
{
	if (!_transform)return;
	
	// 패턴 예고
	static const size_t ParticleNum = 12;
	static const std::wstring ImgKey = L"FIRE_PARTICLE";
	static const float ParticleDuration = 0.5f;
	static const float PatternNotifyTime = 2.f;
	static const int32_t ParticlePlayNumber = 4;
	// launch 시 파티클 위치 , 파티클 스프라이트 인덱스
	std::vector<std::tuple<vec, vec,int32_t>> ParticleInfos;
	vec MyLocation = _transform->_location;
	
	auto ParticleInfoGenerate = [MyLocation]()
		->typename decltype(ParticleInfos)::value_type
	{
		const std::pair<int, int> RandRange = { -100,100};
	
		int32_t Row = math::Rand<int32_t>({0,3});
		vec InitLocation = MyLocation;
		vec RandDir = math::RandVec();
		InitLocation += RandDir* math::Rand<int32_t>(RandRange);
		return { InitLocation , RandDir   ,Row  };
	};
	
	Timer::instance().event_regist_ReWhileDelta(PatternNotifyTime +0.05f, PatternNotifyTime /(float)ParticlePlayNumber ,
		[ParticleInfos, ParticleInfoGenerate]()mutable
	{
		if (!ParticleInfos.empty())
			ParticleInfos.clear();
		
		ParticleInfos.reserve(ParticleNum);
		
		std::generate_n(std::back_inserter(ParticleInfos),
			ParticleNum, (ParticleInfoGenerate));

		for (const auto& [Location, Dir, IDX] : ParticleInfos)
		{
			int ImgLocationX = Location.x;
			int ImgLocationY = Location.y;
			int AnimRowIndex = IDX;

			auto sp_Effect = object_mgr::instance().insert_object<Effect>(
				ImgLocationX, ImgLocationY, ImgKey, layer_type::EEffect,
				5, AnimRowIndex, ParticleDuration, ParticleDuration,
				70, 70, 0.8f, 0.8f , Dir);
		}
	});
	 //  공격 시작
	
	Timer::instance().event_regist(time_event::EOnce, PatternNotifyTime, [MyLocation,NUM]()
	{
		float InitDistance = 100.f;
		float SpawnBetWeenAngle = 360.f / NUM;

		std::shared_ptr<vec> _vec = std::make_shared<vec>(math::dir_from_angle(0));

		Timer::instance().event_regist_ReWhileDelta(6.f, 0.33f, [=]() {
			vec SpawnDirFromBoss = *_vec;
			for (size_t i = 0; i < NUM; ++i)
			{
				auto _FD = object_mgr::instance().insert_object<BossFireDragon>();
				if (!_FD)return;

				vec SpawnLocation = MyLocation + SpawnDirFromBoss * InitDistance;
				_FD->SetUp(SpawnLocation, SpawnDirFromBoss);

				SpawnDirFromBoss =
					math::rotation_dir_to_add_angle(SpawnDirFromBoss,
						SpawnBetWeenAngle);
			}
			*_vec = math::rotation_dir_to_add_angle(*_vec, SpawnBetWeenAngle / 2);
		});
		return true; 
	});

}

void MIDDLE_BOSS::BOSS_SKill_Boomerang(size_t NUM)
{
	if (!_transform)return;
	auto sp_Target =wp_AttackTarget.lock();
	if (!sp_Target)return;
	vec MyLocation = _transform->_location;
	vec InitDis = sp_Target->_transform->_location - MyLocation;
	vec InitDir = InitDis.get_normalize();

	float InitInstanceBetWeenAngle = 90.f / NUM;
	float Boomerang_Init_Distance = 10.f;

	Timer::instance().event_regist_ReWhileDelta(3.f, 1.f, [=]() {
		Timer::instance().event_regist_ReWhileDelta(1.0f, 0.3f, [=]() {
			for (int i = 0; i < NUM; ++i)
			{
				vec CurrentInitDir = math::rotation_dir_to_add_angle(InitDir,
					InitInstanceBetWeenAngle * i);
				auto sp_Boo = object_mgr::instance().insert_object<BossBoomerang>();

				if (!sp_Boo)return;
				sp_Boo->SetUp
				(wp_AttackTarget,
					MyLocation + CurrentInitDir * Boomerang_Init_Distance,
					CurrentInitDir);
			};
		});
	});
}

void MIDDLE_BOSS::BOSS_SKill_Blast()
{
	std::shared_ptr<float> sp_Angle = std::make_shared<float> (0) ;
	std::shared_ptr<int> sp_Num = std::make_shared<int>(3);

	Timer::instance().event_regist_ReWhileDelta(2.1+0.1, 2.1/3, [=,this]() {
		if (!sp_Angle)return;

		for (int i = 0; i < 2; ++i)
		{
			BOSS_Blast_launch(*sp_Num, i * 90 + *sp_Angle);
		}
		*sp_Angle += 90.f /3.f ;
		*sp_Num += 3;
	});
}

void MIDDLE_BOSS::BOSS_Blast_launch(size_t NUM, float launchAngle)
{
	Timer& _Timer = Timer::instance();

	float BlastDistanceMin = 80.f;
	float BlastDistanceBetween = 80.f;
	float BlastSpawnCycle = 0.05f;
	int    IcePilarNum = 2;
	float IcePilarDuration = 4.f;
	int IcePilarDistribution = 300;
	float IcePilarBeetWeen = 100.f;

	// 블라스트 생성 이펙트 한번 뿌려주기
	object_mgr& obj_mgr = object_mgr::instance();
	vec PlayerLocation = _transform->_location;
	vec dis = math::dir_from_angle(launchAngle);
	vec dir = dis.get_normalize();

	for (int i = 0; i < NUM; ++i)
	{
		vec blast_location = PlayerLocation + dir * (BlastDistanceBetween * i + BlastDistanceMin);

		_Timer.event_regist(time_event::EOnce, i * BlastSpawnCycle, [blast_location]()->bool {
			auto BLAST = object_mgr::instance().insert_object<Boss_Blast>();
			BLAST->_transform->_location = blast_location;
			return true;
		});
	};

	vec PilarSpwanLocation = PlayerLocation + dir * (BlastDistanceBetween * NUM);

	_Timer.event_regist(time_event::EOnce, NUM * BlastSpawnCycle, [
		IcePilarBeetWeen, IcePilarDistribution, IcePilarDuration, BlastSpawnCycle, IcePilarNum,
			&_Timer = _Timer, PilarSpwanLocation = PilarSpwanLocation, dir = dir]()->bool {

		_Timer.event_regist(time_event::EOnce, 0.6f,
			[IcePilarBeetWeen, PilarSpwanLocation, IcePilarDistribution, IcePilarDuration, IcePilarNum]()->bool {

			vec left = vec{ -1,0 };
			vec right = vec{ +1,0 };
			vec up = vec{ 0,1 };
			vec down = vec{ 0,-1 };

			vec CurrentDir{};
			// 4방향 기둥 생성
			for (int i = 0; i < 4; ++i)
			{
				switch (i)
				{
				case 0:
					CurrentDir = left;
					break;
				case 1:
					CurrentDir = right;
					break;
				case 2:
					CurrentDir = up;
					break;
				case 3:
					CurrentDir = down;
					break;
				default:
					break;
				}
				for (int i = 0; i < IcePilarNum; ++i)
				{
					auto BLAST = object_mgr::instance().insert_object<Boss_Blast>();
					BLAST->Duration = IcePilarDuration;
					BLAST->_transform->_location = PilarSpwanLocation + (CurrentDir * IcePilarBeetWeen * i);
				}
			}
			return true;
		});

		return true;
	});
}

void MIDDLE_BOSS::SummonMonster()
{
	if (MonsterSpawnCurrentTick > 0)return;
	if (!_transform)return;

	float SummonDistance = 150.f;
	vec BossPos =_transform->_location;

	vec ARHCERLocation = BossPos + vec{ -1,0 }*SummonDistance;
	vec SwordManLocation = BossPos + vec{ 1,0 }*SummonDistance;
		
	Monster::CardEffect(SwordManLocation, L"SUMMON_CARD_SWORDMAN");
	Monster::CardEffect(ARHCERLocation, L"SUMMON_CARD_ARCHER");

	object_mgr::instance().insert_object<SwordMan>(wp_AttackTarget, SwordManLocation);
	object_mgr::instance().insert_object<ARCHER>(wp_AttackTarget, ARHCERLocation);

	MonsterSpawnCurrentTick = MonsterSpawnCycle;
}

void MIDDLE_BOSS::DieAction()
{
	sound_mgr::instance().RandSoundKeyPlay("ENEMY_DIED", { 0,3 }, 1.f);

	auto sp_UIBOSSHPBar = wp_UIBossHPBar.lock();
	if (!sp_UIBOSSHPBar)return;
	sp_UIBOSSHPBar->bDie = true;

	auto sp_wp_UIBossName = wp_UIBossName.lock();
	if (!sp_wp_UIBossName)return;
	sp_wp_UIBossName->bDie = true;
}

void MIDDLE_BOSS::BlastSpawn(size_t NUM)
{
	if (!_transform)return;
	vec InitDir = vec::unit_diagonal_vec();
	vec InitLocation = _transform->_location;
	float BLAST_Init_Distance = 150.f;

	for (int i = 0; i < NUM; ++i)
	{
		object_mgr::instance().insert_object< HomingBlast>(wp_AttackTarget,
			InitLocation + InitDir * BLAST_Init_Distance, InitDir);

		InitDir = math::rotation_dir_to_add_angle(InitDir, 360.f / NUM);
	};
}
