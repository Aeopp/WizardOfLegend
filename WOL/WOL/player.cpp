#include "pch.h"
#include "player.h"
#include "collision_mgr.h"
#include "Input_mgr.h"
#include "timer.h"
#include "Camera.h"
#include "game.h"
#include "shield.h"
#include "object_mgr.h"
#include "Mouse.h"
#include "ICE_Blast.h"
#include "timer.h"
#include "Bmp_mgr.h"
#include "UISkillBar.h"
#include "UIGold.h"
#include "UI_PlayerBar.h"
#include "player_info.h"
#include "UIHPBar.h"
#include "UIMpBar.h"
#include "UISkillIBarIcon.h"
#include "render_component.h"
#include "Color.h"
#include "Bmp_mgr.h"
#include "helper.h"
#include "ICE_Crystal.h"
#include "Bmp.h"
#include "Effect.h"
#include "FireDragon.h"
#include "EffectPlayerAttack.h"
#include "sound_mgr.h"
#include "ScrewBoomerang.h"
#include "helper.h"
#include "BOTTOM_HOLE.h"
#include "UIInventory.h"

void Player::render(HDC hdc, vec camera_pos, vec size_factor)
{

	actor::render(hdc, camera_pos, size_factor);
	
	if (bDebug)
	{
		vec v = _transform->_location;
		auto o = Input_mgr::instance().GetWorldMousePos();
		vec r = *o - v;

		vec n = r.get_normalize();
		helper::TEXTOUT(hdc, 0, 300, L"Angle (Degree) : ", math::radian_to_degree(atan2f(n.y, n.x)));
		helper::TEXTOUT(hdc, 0, 400, L"Distance : ",r.length());

		v -= camera_pos;
		*o -= camera_pos;

		MoveToEx(hdc, v.x, v.y, nullptr);
		LineTo(hdc, o->x, o->y);
	}
};

void Player::initialize()
{
	actor::initialize();
	Bmp_mgr& BMgr = Bmp_mgr::instance();

	AnimDirFileTable[(int)EAnimDir::back] = BMgr.Insert_Bmp(L"BACK_COMPLETE.bmp", L"BACK_COMPLETE");
	AnimDirFileTable[(int)EAnimDir::front] = BMgr.Insert_Bmp(L"FRONT_COMPLETE.bmp", L"FRONT_COMPLETE");
	AnimDirFileTable[(int)EAnimDir::left] = BMgr.Insert_Bmp(L"LEFT_COMPLETE.bmp", L"LEFT_COMPLETE");
	AnimDirFileTable[(int)EAnimDir::right] = BMgr.Insert_Bmp(L"RIGHT_COMPLETE.bmp", L"RIGHT_COMPLETE");

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EPlayer, ERect);
	auto sp_collision = _collision_component.lock();

	if (!sp_collision)return;
	sp_collision->bSlide = true;
	sp_collision->_size = { 30.f,30.0f };
	sp_collision->bHitEffect = false;

	_Camera = object_mgr::instance()._Camera;

	_player_info = game::instance()._player_info;
	if (!_player_info)return;

	PaintSizeX = 180;
	PaintSizeY = 182;

	float Scale = 0.85f;

	//Anim SetUp
	{
		_render_component = render_component::LoadRenderComponent_SP
		(L"FRONT_COMPLETE.bmp", L"FRONT_COMPLETE");
		// 174 182
		_render_component->Default_Src_Paint_Size = vec{ PaintSizeX,PaintSizeY };
		_render_component->Dest_Paint_Size = vec{ PaintSizeX * Scale,PaintSizeY * Scale };
		_render_component->_ColorKey = COLOR::MRGENTA();
		_render_component->_Img_src = RECT{ 0,0,PaintSizeX,PaintSizeY };
		_render_component->_Anim.SetAnimationClip(
			{ 1 ,10,8,8,8,2,7 }, 0.1f);
	}

	make_skill_bar();

	make_gold_UI();

	make_player_bar();

	make_skillbar_icon(ESkill::ARMOR);
	make_skillbar_icon(ESkill::BLAST);
	make_skillbar_icon(ESkill::CRYSTAL);
	make_skillbar_icon(ESkill::FIRE);

	_speed = 400.f;

	_Shadow.correction = { +0,(PaintSizeY * 0.55) /2.f };
	_Shadow.world_size_correction = { 10,0 };


	Timer::instance().event_regist(time_event::ERemaingWhile, FLT_MAX,
		[&bAttack = _player_info->bAttack,&AttackCheck = _player_info->CurrentAttackDuration]()->bool{
		AttackCheck -= DeltaTime;
		if (AttackCheck < 0)bAttack = false;
		else  bAttack = true;

		return true;
	});


	/*void late_initialize(int ImgLocationX, int ImgLocationY,
	std::wstring ImgKey, layer_type layer_ID, int AnimColNum,
	int AnimRowIndex, float Duration, float AnimDuration,
	int PaintSizeX, int PaintSizeY, float ScaleX, float ScaleY);*/
	auto sp_nAttack = object_mgr::instance().insert_object<EffectPlayerAttack>(
		0, 0, L"WOL_NORMAL_ATTACK", layer_type::EEffect, 4,
		0, _player_info->DefaultAttackDuration, _player_info->AttackAnimDuration,
		200, 200, 0.8f, 0.8f);

	if (!sp_nAttack) return;

	NormalAttack = sp_nAttack;

	sp_nAttack->_owner = _ptr;

	ObjectTag = object::Tag::player;

	CurrentInvincibletime = DefaultInvincibletime = 0.3f;


	wp_Inventory = object_mgr::instance().insert_object<UIInventory>();
};

Event Player::update(float dt)
{
	Event _E = object::update(dt);
	CurrentInvincibletime -= dt;
	IceCrystalTick -= dt;

	player_check(dt);
	if (!_player_info) return Event::Die;

	_player_info->AddHp(_player_info->HpRegenerationAtSec * dt);
	_player_info->AddMp(_player_info->MpRegenerationAtSec* dt);

	_player_info->SkillCurrentICEBlastCoolTime = 
	min(_player_info->SkillCurrentICEBlastCoolTime+dt, _player_info->SkillICEBlastCoolTime);  

	_player_info->SkillCurrentBoomerangNum= 
	min(_player_info->SkillCurrentBoomerangNum + dt, _player_info->SkillBoomerangMaxNum);

	_player_info->SkillCurrentICECrystalCoolTime= 
	min(_player_info->SkillCurrentICECrystalCoolTime + dt, _player_info->SkillICECrystalCoolTime);

	_player_info->SkillCurrentShieldCoolTime = 
	min(_player_info->SkillCurrentShieldCoolTime + dt, _player_info->SkillShieldCoolTime);

	if (_player_info->GetMP() >= _player_info->max_mp)
	{
		SOUNDPLAY("ULT_ON", 1.f, false);
	}
	

	return _E;
}
void Player::Hit(std::weak_ptr<object> _target)
{
	if (CurrentInvincibletime > 0)return;
	auto sp_target = _target.lock();
	if (!sp_target)return;
	if (!sp_target->bAttacking)return;
	if (sp_target->ObjectTag == object::Tag::player_attack)return;
	//if (sp_target->ObjectTag == object::Tag::monster)return;

	//if (sp_target->ObjectTag == object::Tag::monster && sp_target->UniqueID != EObjUniqueID::EWizardBall)return;

	if (_player_info->bProtected == true){
		sound_mgr::instance().Play("PLAYER_HITED_1", false, 1.f);
		return;
	}

	sound_mgr::instance().Play("PLAYER_HITED_1", false, 1.f);
	RAND_SOUNDPLAY("HIT_SOUND_NORMAL", { 1,2 }, 1.f, false);

	int Dice = math::Rand<int>(_player_info->MissRange);
	if (Dice == 0)
	{
		MissMsg();
		return;
	}
	Timer& _Timer = Timer::instance();

	if(_player_info->GetHP()>0)
	{
		CurrentInvincibletime = DefaultInvincibletime;

		_Shadow.CurrentShadowState = EShadowState::NORMAL;
		_player_info->bHit = true;
		if (!_player_info->bDash)
			_render_component->ChangeUnstoppableAnim(AnimTable::hit, 0.5f, AnimTable::idle);
			collision_mgr::instance().HitEffectPush(_transform->_location, 0.5f);
			_Timer.event_regist(time_event::EOnce, 0.5f,
			[&bHit = _player_info->bHit]() 
			{
				bHit = false; 
			return true; 
			});
	}
	else
	{
		_Shadow.CurrentShadowState = EShadowState::NORMAL;
		_render_component->wp_Image = AnimDirFileTable[(int)EAnimDir::front];
		_render_component->ChangeUnstoppableAnim(AnimTable::dead, 1.f,	AnimTable::dead);
	
	//	Timer::instance().time_scale = 0.1f;
		sound_mgr::instance().Play("PLAYER_DIE", false, 1.f);
	///*	_Timer.event_regist(EOnce, 1.f, []() {
	//		MessageBox(game::hWnd, L" 사망하셨습니다 다시 도전해 보세요. ", L"DEAD!!",
	//			MB_OK);
	//		return true;
	//		});;
	//	_Timer.time_scale = 1.0f;*/
	//	DeltaTime = 0.0f;

		_player_info->SetHp(_player_info->max_hp);
		_player_info->SetMp(_player_info->max_mp);
	}
	
	float Atk = math::Rand<int>(sp_target->Attack);

	_player_info->AddHp(-Atk);

	vec randvec = math::RandVec();
	randvec.y = (abs(randvec.y));
	vec v = _transform->_location;
	v.y -= 35;
	v.x += math::Rand<int>({ -40,+40 });

	object_mgr::instance().TextEffectMap[RGB(255, 50, 64)].
		push_back({ v ,vec{0,1}*3,
		1.f,int(Atk),std::to_wstring((int)Atk) });

	Camera_Shake(Atk, 
		(_transform->_location - sp_target->_transform->_location).get_normalize(),
		Atk*0.01);


};

void Player::StateCheck()
{

}

void Player::temp(float temp)
{
	MessageBox(game::instance().hWnd, __FUNCTIONW__, __FUNCTIONW__, MB_OK);
}

void Player::InputDirSpriteChange(vec Dir)
{
	math::EDir _Dir = math::checkDir(Dir);

	switch (_Dir)
	{
	case math::EDir::left:
		_render_component->ChangeImg(AnimDirFileTable[(int)EAnimDir::left]);
		break;
	case math::EDir::right:
		_render_component->ChangeImg(AnimDirFileTable[(int)EAnimDir::right]);
		break;
	case math::EDir::up:
		_render_component->ChangeImg(AnimDirFileTable[(int)EAnimDir::front]);
		break;
	case math::EDir::down:
		_render_component->ChangeImg(AnimDirFileTable[(int)EAnimDir::back]);
		break;
	default:
		break;
	}
}

void Player::MakeShield()
{
	if (!_player_info)return;
	if (_player_info->bDash)return;
	if (_player_info->SkillCurrentShieldCoolTime < _player_info->SkillShieldCoolTime)return;
	_player_info->SkillCurrentShieldCoolTime =0;

	Input_mgr& _Input = Input_mgr::instance();
	object_mgr& _obj_mgr = object_mgr::instance();

	float degree= 360.0f / 8.f;
	
	for (int i = 0; i < 8; ++i)
	{
		if (!_transform)return;

		Transform _Transform;
		_Transform._location = _transform->_location;
		_Transform._dir = math::dir_from_angle(degree * i);

		auto _shield = _obj_mgr.insert_object<shield>(std::move(_Transform));
		if (!_shield) return;
		_shield->_owner = _ptr;
		_shield->duration = _player_info->ShieldDuration;
		_shield->Angle = degree * i;
		_shield->CalcIdx();
	}
	// 지속시간 종료시 쉴드 무적 해제
	Timer::instance().event_regist(time_event::EOnce, _player_info->ShieldDuration, 
	[&PInfo = _player_info]() {PInfo->bProtected = false; return true;  });

	_player_info->bProtected = true;

	_player_info->bIdle = false;
	_player_info->bAttack = true;

	_Shadow.CurrentShadowState = EShadowState::BIG;
	if (!_player_info)return;
	if (_player_info->bDash)return;

	object_mgr& _object_mgr = object_mgr::instance();

	Anim& MyAnim = _render_component->_Anim;
	_player_info->CurrentAttackDuration = _player_info->SkillShieldMotionDuration;

	_render_component->ChangeAnim(AnimTable::attack2, _player_info->SkillShieldMotionDuration);

	vec dir{ math::Rand<float>({ -10,+10 }), math::Rand<float>({ -0,+0 }) };
	Camera_Shake(15, dir, 0.5f);
	//_player_info->AddMp(-200);

	sound_mgr::instance().Play("GAIA_ARMOR_START", false, 1.f);
}

void Player::ICE_BLAST(int Num)
{
	if (!_player_info)return;
	if (_player_info->bDash)return;
	if (!_transform)return;
	if (_player_info->SkillCurrentICEBlastCoolTime < _player_info->SkillICEBlastCoolTime)return;
	_player_info->SkillCurrentICEBlastCoolTime = 0;

	Input_mgr& _Input = Input_mgr::instance();

	auto oMousePos= _Input.GetWorldMousePos();
	if (!oMousePos) return;

	Timer& _Timer = Timer::instance();

	float BlastDistanceBetween = 80.f;
	float BlastSpawnCycle = 0.05f;
	int    IcePilarNum = 2;
	float IcePilarDuration = 4.f;
	int IcePilarDistribution = 300;
	float IcePilarBeetWeen = 100.f;

	// 블라스트 생성 이펙트 한번 뿌려주기
	object_mgr& obj_mgr = object_mgr::instance();
	vec PlayerLocation = _transform->_location;
	vec dis = *oMousePos - PlayerLocation;
	vec dir = dis.get_normalize();

	for (int i = 0; i < Num; ++i)
	{
		vec blast_location = PlayerLocation + dir * (BlastDistanceBetween * i);

		_Timer.event_regist(time_event::EOnce, i * BlastSpawnCycle, [blast_location]()->bool {
			auto BLAST = object_mgr::instance().insert_object<ICE_Blast>();
			BLAST->_transform->_location = blast_location;
			return true;
			});
	};

	vec PilarSpwanLocation = PlayerLocation + dir * (BlastDistanceBetween * Num);

	_Timer.event_regist(time_event::EOnce, Num * BlastSpawnCycle, [
		IcePilarBeetWeen,IcePilarDistribution,IcePilarDuration,BlastSpawnCycle, IcePilarNum,
		&_Timer = _Timer, PilarSpwanLocation = PilarSpwanLocation, dir = dir]()->bool {

				_Timer.event_regist(time_event::EOnce, 0.6f, 
				[IcePilarBeetWeen,PilarSpwanLocation, IcePilarDistribution,IcePilarDuration ,IcePilarNum]()->bool {

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
								auto BLAST = object_mgr::instance().insert_object<ICE_Blast>();
								BLAST->Duration = IcePilarDuration;
								BLAST->_transform->_location = PilarSpwanLocation + (CurrentDir * IcePilarBeetWeen * i);
							}
						}
					return true; 
					});
			
		return true;
		});

	_Shadow.CurrentShadowState = EShadowState::BIG;
	Anim& MyAnim = _render_component->_Anim;
	_render_component->ChangeAnim(AnimTable::attack2, _player_info->SkillICEBlastMotionDuration);
	_player_info->bIdle = false;
	_player_info->bAttack = true;	
	_player_info->CurrentAttackDuration = _player_info->DefaultAttackDuration;

	vec Dir{ math::Rand<float>({ 0,0 }), math::Rand<float>({ -10,+10 }) };
	Camera_Shake(1, Dir, 0.1f);

	//_player_info->AddMp(-150);

}
void Player::Camera_Shake(float force,vec dir,float duration)
{
	auto Cam = _Camera.lock();

	if (!Cam)
	{
		this->_Camera = object_mgr::instance()._Camera;
		return;
	};

	Cam->camera_shake(force, dir ,duration);
};

void Player::BindingSkillCheckCast(int SlotIdx)
{
	if (!UIInventory::SlotInfoMap[SlotIdx].bAcquire)return;

	ESkill _Skill = UIInventory::SlotInfoMap[SlotIdx]._Skill;

	switch (_Skill)
	{
	case ESkill::Normal:
		break;
	case ESkill::Dash:
		break;
	case ESkill::FIRE:
		SkillRotBoomerang();

		break;
	case ESkill::BLAST:
		ICE_BLAST(8);
		break;
	case ESkill::CRYSTAL:
		if (_player_info->GetMP() >= _player_info->max_mp){
			SkillUlti();
		}
		else{
			SkillIceCrystal(3);
		}
		break;
	case ESkill::ARMOR:
		MakeShield();
		break;
	case ESkill::BOOMERANG:
		break;
	default:
		break;
	}
}

void Player::player_check(float dt)
{
	Input_mgr& _Input = Input_mgr::instance();
	auto sp_Inven = wp_Inventory.lock();

	if (_Input.Key_Down('I'))
	{
		if (sp_Inven)
		{
			bInvenControl = sp_Inven->bInventoryOpen = !sp_Inven->bInventoryOpen;
			if (bInvenControl)
				SOUNDPLAY("OPEN_INVENTORY");
			else
				SOUNDPLAY("CLOSE_INVENTORY");
		}
	}

	if (bInvenControl)return;

	

	_player_info->bIdle = true;

	if (!_player_info)return;

	Player_Move(dt);


	if (_Input.Key_Down('Q'))
	{
		BindingSkillCheckCast(3);
	}

	if (_Input.Key_Down(VK_LBUTTON))
	{

	   Attack();
	}

	if (_Input.Key_Down(VK_RBUTTON)) 
	{
		BindingSkillCheckCast(2);

	}

	if (_Input.Key_Down('R'))
	{
		BindingSkillCheckCast(5);
	}

	if (_Input.Key_Down('E'))
	{
		BindingSkillCheckCast(4);
	}

	//if (_Input.Key_Down('Z')) {
	//	SkillBoomerang();
	//}
	//if (_Input.Key_Down('X')) {
	//	MultiBoomerang(8);
	//}

	//if (_Input.Key_Down('C')) {
	//	SkillBoomerang();
	//}
	//if (_Input.Key_Down('V')) {
	//	MultiRotBoomerang(6);
	//}
	//if (_Input.Key_Down('B')) {
	//	SkillScrewBoomerang();
	//}
	//if (_Input.Key_Down('N')) {
	//	MultiScrewBoomerang(8);
	//}

	if (_Input.Key_Down(VK_SPACE))
	{
		Dash(_player_info->dash_speed);
		
	}

	if (_player_info->bIdle && !_player_info->bAttack && !_player_info->bDash)
	{
		_Shadow.CurrentShadowState = EShadowState::NORMAL;

		_render_component->ChangeAnim(AnimTable::idle, 0.1f, AnimTable::idle);
	}
};

void Player::SkillBoomerang()
{
	if (!_player_info)return;
	if (_player_info->bDash)return;
	if (_player_info->SkillCurrentBoomerangNum< _player_info->SkillBoomerangMaxNum)return;
	_player_info->SkillCurrentICECrystalCoolTime -= 1;

	object_mgr& _object_mgr = object_mgr::instance();

	auto _Boom= _object_mgr.insert_object<Boomerang>();
	if (!_Boom)return;
	if (!_Boom->_transform)return;

	float InitSpeed = 1.f;

	vec MyLocation = _transform->_location;
	vec Dis = *Input_mgr::instance().GetWorldMousePos() - MyLocation;
	vec Dir = Dis.get_normalize(); 

	_Boom->_transform->_dir = Dir;
	_Boom->_transform->_location  = MyLocation  +  Dir * InitSpeed;
	_Boom->CalcImgAngle(math::radian_to_degree(atan2f(Dir.y, Dir.x)));

	_player_info->bIdle = false;
	_Shadow.CurrentShadowState = EShadowState::BIG;

	Anim& MyAnim = _render_component->_Anim;
	_player_info->bAttack = true;
	_player_info->CurrentAttackDuration = _player_info->SkillBoomerangMotionDuration;

	int AttackAnimNum =math::Rand<int>({ 1,2 });

	if (AttackAnimNum == 1)
	{
		_render_component->ChangeAnim(AnimTable::attack1,
			_player_info->SkillBoomerangMotionDuration);
	}
	else 
	{
		_render_component->ChangeAnim(AnimTable::attack2,
			_player_info->SkillBoomerangMotionDuration);
	}

	vec dir{ math::Rand<float>({ -7,+7 }), math::Rand<float>({ -7,+7 }) };

	Camera_Shake(1, dir, 0.2f);

	//_player_info->AddMp(-50);

}

void Player::SkillIceCrystal(uint32_t Num)
{
	if (!_player_info)return;
	if (_player_info->bDash)return;
	if (_player_info->SkillCurrentICECrystalCoolTime < _player_info->SkillICECrystalCoolTime)return;
	_player_info->SkillCurrentICECrystalCoolTime = 0;

	object_mgr& _object_mgr = object_mgr::instance();

	float degree = 360.0f / Num;

	for (int i = 0; i < Num; ++i)
	{
		auto _Ice = _object_mgr.insert_object<ICE_Crystal>();

		if (!_Ice)return;
		_Ice->_owner = _ptr;
		_Ice->_transform->_dir = math::dir_from_angle(degree * i);
	}
	_player_info->bIdle = false;
	_Shadow.CurrentShadowState = EShadowState::BIG;

	Anim& MyAnim = _render_component->_Anim;
	_player_info->bAttack = true;
	_player_info->CurrentAttackDuration = _player_info->SkillICECrystalMotionDuration;

	_render_component->ChangeAnim(AnimTable::attack1, _player_info->SkillICECrystalMotionDuration);

	vec dir{ math::Rand<float>({ -7,+7 }), math::Rand<float>({ -7,+7 }) };
	Camera_Shake(2, dir, 0.2f);

	//_player_info->AddMp(-100);
}

void Player::SkillUlti()
{
	if (!_player_info)return;
	if (_player_info->bDash)return;
	if (_player_info->SkillCurrentICECrystalCoolTime < _player_info->SkillICECrystalCoolTime)return;
	_player_info->SkillCurrentICECrystalCoolTime = 0;

	object_mgr& _object_mgr = object_mgr::instance();

	float degree = 360.0f / 6;

	for (int i = 0; i < 6; ++i)
	{
		auto _Ice = _object_mgr.insert_object<ICE_Crystal>();

		if (!_Ice)return;
		_Ice->_owner = _ptr;
		_Ice->_transform->_dir = math::dir_from_angle(degree * i);
		_Ice->RotationSpeedDegree = 720.f;
		_Ice->_speed = 720.f;
		_Ice->Duration = 10.f; 
	}
	_player_info->bIdle = false;
	_Shadow.CurrentShadowState = EShadowState::BIG;

	Anim& MyAnim = _render_component->_Anim;
	_player_info->bAttack = true;
	_player_info->CurrentAttackDuration = _player_info->SkillICECrystalMotionDuration;

	_render_component->ChangeAnim(AnimTable::attack1, _player_info->SkillICECrystalMotionDuration);

	vec dir{ math::Rand<float>({ -7,+7 }), math::Rand<float>({ -7,+7 }) };
	Camera_Shake(2, dir, 0.2f);

	_player_info->AddMp(-_player_info->max_mp);

	SOUNDPLAY("ULT_USE", 1.f, false);
}


void Player::SkillFireDragon()
{
	if (!_player_info)return;
	if (_player_info->bDash)return;
	static int UpDown = 1;

	 auto mp  =  Input_mgr::instance().GetWorldMousePos(); 
	 if (!mp)return;

	 vec FireDir = (*mp - _transform->_location).get_normalize();
	float FireInitDistance = 10.f;
	auto _Fire = object_mgr::instance().insert_object<FireDragon>();

	if (!_Fire)return;

	_Fire->_transform->_location = _transform->_location + (FireDir * FireInitDistance);
	_Fire->rotation_center = _transform->_location + (FireDir * FireInitDistance);
	_Fire->_transform->_dir = FireDir;
	_Fire->rotation_center_dir = FireDir;
	_Fire->Cross = math::rotation_dir_to_add_angle(FireDir, 90.f);
	_Fire->Updown = UpDown;
	_player_info->bIdle = false;

	Anim& MyAnim = _render_component->_Anim;
	_Shadow.CurrentShadowState = EShadowState::MIDDLE;

	_player_info->CurrentAttackDuration = _player_info->SkillFireDragonMotionDuration;

	vec dis = *Input_mgr::instance().GetWorldMousePos() - _transform->_location;

	math::EDir _Dir = math::checkDir(dis.get_normalize());

	switch (_Dir)
	{
	case math::EDir::left:
		_render_component->ChangeImg(AnimDirFileTable[(int)EAnimDir::left]);
		break;
	case math::EDir::right:
		_render_component->ChangeImg(AnimDirFileTable[(int)EAnimDir::right]);
		break;
	case math::EDir::up:
		_render_component->ChangeImg(AnimDirFileTable[(int)EAnimDir::front]);
		break;
	case math::EDir::down:
		_render_component->ChangeImg(AnimDirFileTable[(int)EAnimDir::back]);
		break;
	default:
		break;
	}

	if (UpDown > 0)
		_render_component->ChangeAnim(AnimTable::attack1, _player_info->SkillFireDragonMotionDuration);
	else 
		_render_component->ChangeAnim(AnimTable::attack2, _player_info->SkillFireDragonMotionDuration);
	
	_player_info->bAttack = true;

	vec dir{ math::Rand<float>({ -3,+3 }), math::Rand<float>({ -3,+3 }) };
	Camera_Shake(5, dir, 0.25f);

	UpDown *= -1;

	//_player_info->AddMp(-25);
}

void Player::MultiBoomerang(int Num)
{
	if (!_player_info)return;
	if (_player_info->bDash)return;

	object_mgr& _object_mgr = object_mgr::instance();


	float InitSpeed = 10.f;

	vec MyLocation = _transform->_location;
	vec Dis = *Input_mgr::instance().GetWorldMousePos() - MyLocation;
	vec Dir = Dis.get_normalize();

	for (int i = 0; i < Num; ++i)
	{
		auto _Boom = _object_mgr.insert_object<Boomerang>();
		if (!_Boom)return;
		if (!_Boom->_transform)return;
		vec InitDir = math::rotation_dir_to_add_angle(Dir, (360.f / Num) * i); 
		_Boom->_transform->_dir = InitDir; 
		_Boom->_transform->_location = MyLocation + InitDir * InitSpeed;
		_Boom->CalcImgAngle(math::radian_to_degree(atan2f(InitDir.y, InitDir.x)));
	}
	_player_info->bIdle = false;
	_Shadow.CurrentShadowState = EShadowState::BIG;

	Anim& MyAnim = _render_component->_Anim;
	_player_info->bAttack = true;
	_player_info->CurrentAttackDuration = _player_info->SkillBoomerangMotionDuration;

	int AttackAnimNum = math::Rand<int>({ 1,2 });

	if (AttackAnimNum == 1)
	{
		_render_component->ChangeAnim(AnimTable::attack1,
			_player_info->SkillBoomerangMotionDuration);
	}
	else
	{
		_render_component->ChangeAnim(AnimTable::attack2,
			_player_info->SkillBoomerangMotionDuration);
	}

	vec dir{ math::Rand<float>({ -7,+7 }), math::Rand<float>({ -7,+7 }) };

	Camera_Shake(8, dir, 0.4f);

	//_player_info->AddMp(-50);
}
void Player::SkillRotBoomerang()
{
	if (!_player_info)return;
	if (_player_info->bDash)return;
	if (_player_info->SkillCurrentBoomerangNum < 1.f)return;
	_player_info->SkillCurrentBoomerangNum -= 1;
	object_mgr& _object_mgr = object_mgr::instance();

	auto _Boom = _object_mgr.insert_object<RotationBoomerang>();
	if (!_Boom)return;
	if (!_Boom->_transform)return;

	float InitSpeed = 0.5f;

	vec MyLocation = _transform->_location;
	vec Dis = *Input_mgr::instance().GetWorldMousePos() - MyLocation;
	vec Dir = Dis.get_normalize();

	_Boom->_transform->_dir = Dir;
	_Boom->_transform->_location = MyLocation + Dir * InitSpeed;
	_Boom->CalcImgAngle(math::radian_to_degree(atan2f(Dir.y, Dir.x)));

	_player_info->bIdle = false;
	_Shadow.CurrentShadowState = EShadowState::BIG;

	Anim& MyAnim = _render_component->_Anim;
	_player_info->bAttack = true;
	_player_info->CurrentAttackDuration = _player_info->SkillBoomerangMotionDuration;

	int AttackAnimNum = math::Rand<int>({ 1,2 });

	InputDirSpriteChange(Dir);

	if (AttackAnimNum == 1)
	{
		_render_component->ChangeAnim(AnimTable::attack1,
			_player_info->SkillBoomerangMotionDuration);
	}
	else
	{
		_render_component->ChangeAnim(AnimTable::attack2,
			_player_info->SkillBoomerangMotionDuration);
	}

	vec dir{ math::Rand<float>({ -7,+7 }), math::Rand<float>({ -7,+7 }) };

	Camera_Shake(1, dir, 0.2f);

//	_player_info->AddMp(-50);
}
void Player::MultiRotBoomerang(int Num)
{
	if (!_player_info)return;
	if (_player_info->bDash)return;

	object_mgr& _object_mgr = object_mgr::instance();


	float InitSpeed = 10.f;

	vec MyLocation = _transform->_location;
	vec Dis = *Input_mgr::instance().GetWorldMousePos() - MyLocation;
	vec Dir = Dis.get_normalize();

	for (int i = 0; i < Num; ++i)
	{
		auto _Boom = _object_mgr.insert_object<RotationBoomerang>();
		if (!_Boom)return;
		if (!_Boom->_transform)return;
		vec InitDir = math::rotation_dir_to_add_angle(Dir, (360.f / Num) * i);
		_Boom->_transform->_dir = InitDir;
		_Boom->_transform->_location = MyLocation + InitDir * InitSpeed;
		_Boom->CalcImgAngle(math::radian_to_degree(atan2f(InitDir.y, InitDir.x)));
	}
	_player_info->bIdle = false;
	_Shadow.CurrentShadowState = EShadowState::BIG;

	Anim& MyAnim = _render_component->_Anim;
	_player_info->bAttack = true;
	_player_info->CurrentAttackDuration = _player_info->SkillBoomerangMotionDuration;

	int AttackAnimNum = math::Rand<int>({ 1,2 });

	if (AttackAnimNum == 1)
	{
		_render_component->ChangeAnim(AnimTable::attack1,
			_player_info->SkillBoomerangMotionDuration);
	}
	else
	{
		_render_component->ChangeAnim(AnimTable::attack2,
			_player_info->SkillBoomerangMotionDuration);
	}

	vec dir{ math::Rand<float>({ -7,+7 }), math::Rand<float>({ -7,+7 }) };

	Camera_Shake(4, dir, 0.3f);

	//_player_info->AddMp(-50);

	sound_mgr::instance().Play("ULT_USE", false, 1.f);
	SkillInCastSlowTime(0.25f, 0.5f);
}

void Player::SkillScrewBoomerang()
{
	if (!_player_info)return;
	if (_player_info->bDash)return;
	if (_player_info->SkillCurrentBoomerangNum < 1.f)return;
	_player_info->SkillCurrentBoomerangNum -= 1;

	object_mgr& _object_mgr = object_mgr::instance();

	float InitSpeed = 0.5f;

	vec MyLocation = _transform->_location;
	vec Dis = *Input_mgr::instance().GetWorldMousePos() - MyLocation;
	vec Dir = Dis.get_normalize();

	auto _Boom = _object_mgr.insert_object<ScrewBoomerang>
	(MyLocation + Dir * InitSpeed,Dir);

	if (!_Boom)return;
	if (!_Boom->_transform)return;

	_Boom->_transform->_dir = Dir;
	_Boom->_transform->_location = MyLocation + Dir * InitSpeed;
	_Boom->CalcImgAngle(math::radian_to_degree(atan2f(Dir.y, Dir.x)));

	_player_info->bIdle = false;
	_Shadow.CurrentShadowState = EShadowState::BIG;

	Anim& MyAnim = _render_component->_Anim;
	_player_info->bAttack = true;
	_player_info->CurrentAttackDuration = _player_info->SkillBoomerangMotionDuration;

	int AttackAnimNum = math::Rand<int>({ 1,2 });

	if (AttackAnimNum == 1)
	{
		_render_component->ChangeAnim(AnimTable::attack1,
			_player_info->SkillBoomerangMotionDuration);
	}
	else
	{
		_render_component->ChangeAnim(AnimTable::attack2,
			_player_info->SkillBoomerangMotionDuration);
	}

	vec dir{ math::Rand<float>({ -7,+7 }), math::Rand<float>({ -7,+7 }) };

	Camera_Shake(1, dir, 0.2f);

//	_player_info->AddMp(-50);
}

void Player::MultiScrewBoomerang(int Num)
{
	if (!_player_info)return;
	if (_player_info->bDash)return;

	object_mgr& _object_mgr = object_mgr::instance();


	float InitSpeed = 10.f;

	vec MyLocation = _transform->_location;
	vec Dis = *Input_mgr::instance().GetWorldMousePos() - MyLocation;
	vec Dir = Dis.get_normalize();

	for (int i = 0; i < Num; ++i)
	{
		vec InitDir = math::rotation_dir_to_add_angle(Dir, (360.f / Num) * i);
		auto _Boom = _object_mgr.insert_object<ScrewBoomerang>(
		MyLocation + InitDir * InitSpeed, InitDir);
		if (!_Boom)return;
		if (!_Boom->_transform)return;
		_Boom->_transform->_dir = InitDir;
		_Boom->_transform->_location = MyLocation + InitDir * InitSpeed;
		_Boom->CalcImgAngle(math::radian_to_degree(atan2f(InitDir.y, InitDir.x)));
	}
	_player_info->bIdle = false;
	_Shadow.CurrentShadowState = EShadowState::BIG;

	Anim& MyAnim = _render_component->_Anim;
	_player_info->bAttack = true;
	_player_info->CurrentAttackDuration = _player_info->SkillBoomerangMotionDuration;

	int AttackAnimNum = math::Rand<int>({ 1,2 });

	if (AttackAnimNum == 1)
	{
		_render_component->ChangeAnim(AnimTable::attack1,
			_player_info->SkillBoomerangMotionDuration);
	}
	else
	{
		_render_component->ChangeAnim(AnimTable::attack2,
			_player_info->SkillBoomerangMotionDuration);
	}

	vec dir{ math::Rand<float>({ -7,+7 }), math::Rand<float>({ -7,+7 }) };

	Camera_Shake(8, dir, 0.2f);

//	_player_info->AddMp(-50);

	sound_mgr::instance().Play("ULT_USE", false, 1.f);
	SkillInCastSlowTime(0.25f, 0.3f);

}

void Player::GetSkill()
{
	sound_mgr::instance().Play("GET_SKILL", false, 1.f);
}

void Player::SkillInCastSlowTime(float Duration,float SlowTimeScale)
{
	Timer& _Timer = Timer::instance();
	_Timer.time_scale = SlowTimeScale;
	Timer::instance().event_regist(time_event::EOnce,
		Duration / SlowTimeScale, []()->bool
		{
			Timer::instance().time_scale = 1.f;
			return true;
		});
}

void Player::CheckDirInput()
{
	Input_mgr& _Input = Input_mgr::instance();
	if (_Input.Key_Pressing('D'))
	{
		if (_Input.Key_Pressing('W'))
		{

			_transform->_dir = vec::unit_diagonal_vec();
			_transform->_dir.y *= -1;
		}
		else if (_Input.Key_Pressing('S'))
		{

			_transform->_dir = vec::unit_diagonal_vec();
		}
		else
		{
			_transform->_dir = vec{ +1,0 };
		}
	}
	else if (_Input.Key_Pressing('A'))
	{
		if (_Input.Key_Pressing('W'))
		{

			_transform->_dir = vec::unit_diagonal_vec();
			_transform->_dir *= -1;
		}
		else if (_Input.Key_Pressing('S'))
		{

			_transform->_dir = vec::unit_diagonal_vec();
			_transform->_dir.x *= -1;
		}
		else
		{

			_transform->_dir = vec{ -1,0 };
		}
	}
	else if (_Input.Key_Pressing('W'))
	{

		_transform->_dir = vec{ 0,-1 };
	}
	else if (_Input.Key_Pressing('S'))
	{
		_transform->_dir = vec{ 0,+1 };
	}
}

void Player::make_gold_UI()
{
	auto gold_ui= object_mgr::instance().insert_object<UIGold>();
	if (!gold_ui)return;
	gold_ui->_owner = _ptr;
	gold_ui->_player_info = _player_info;
}

void Player::make_skill_bar()
{
	auto _skbar =object_mgr::instance().insert_object<UISkillBar>();
	if (!_skbar)return;
	_skbar->_owner = _ptr;
}

void Player::make_player_bar()
{
	object_mgr& _object_mgr = object_mgr::instance();

	auto UI = _object_mgr.insert_object<UI_PlayerBar>();
	if (!UI)return;
	UI->_owner = _ptr;
	if (!_player_info)return;
	UI->_player_info = _player_info;

	UI->late_initialize();

	auto HBar = object_mgr::instance().insert_object<UIHPBar>();

	HBar->current = _player_info->GetHP();
	HBar->goal_time = 0.5f;
	HBar->max = _player_info->max_hp;
	HBar->wp_info = _player_info;

	auto MBar = object_mgr::instance().insert_object<UIMPBar>();

	MBar->current = _player_info->GetMP();
	MBar->goal_time = 0.5f;
	MBar->max = _player_info->max_mp;
	MBar->wp_info = _player_info;
}

void Player::make_skillbar_icon(ESkill _eSkill)
{
	std::shared_ptr< UISkillIBarIcon> USBI{};

	switch (_eSkill)

	{
	case ESkill::FIRE:
		USBI = object_mgr::instance().insert_object<UISkillIBarIcon>(
			vec{ 202,838 }, L"FIRE_DRAGON_SKILLBAR.bmp");
		USBI->wp_PlayerInfo = _player_info;
		USBI->CurrentSlotIdx = 2;
		break;
	case ESkill::BLAST:
		USBI = object_mgr::instance().insert_object<UISkillIBarIcon>(
			vec{ 259,838 }, L"ICE_BLAST_SKILLBAR.bmp");
		USBI->wp_PlayerInfo = _player_info;
		USBI->CurrentSlotIdx = 3;
		break;
	case ESkill::CRYSTAL:
		USBI = object_mgr::instance().insert_object<UISkillIBarIcon>(
			vec{ 315,838 }, L"ICE_KRYSTAL_SKILLBAR.bmp");
		USBI->wp_PlayerInfo = _player_info;
		USBI->CurrentSlotIdx = 4;
		break;
	case ESkill::ARMOR:
		USBI = object_mgr::instance().insert_object<UISkillIBarIcon>(
			vec{ 315 + 57,838 }, L"GAIA_ARMOR_SKILLBAR.bmp");
		USBI->wp_PlayerInfo = _player_info;
		USBI->CurrentSlotIdx = 5;
		break;
	default:
		break;
	}
};

void Player::Dash(float speed)
{
	if (!_player_info)return;
	if (_player_info->bDash)return;
	if (_player_info->bHit) return;

	_player_info->bDash = true;
	CheckDirInput();
	InputDirSpriteChange(_transform->_dir);
	_player_info->bIdle = false;
	_Shadow.CurrentShadowState = EShadowState::BIG;

	Timer::instance().event_regist(time_event::EOnce, _player_info->DashDuration,
		[&bDash = _player_info->bDash]()->bool{
		bDash = false; return true; });

	Timer::instance().event_regist(time_event::ERemaingWhile, _player_info->DashDuration,
		std::move([Transform=this->_transform,speed]()->bool {
		if (!Transform)return false;

		Transform->_location += Transform->_dir *(speed*DeltaTime);
		return true;
	}));


	_render_component->ChangeUnstoppableAnim(AnimTable::dash,
	_player_info->AnimDashDuration, AnimTable::idle);

	vec dir{ math::Rand<float>({ -3,+3 }), math::Rand<float>({ -3,+3 }) };
	Camera_Shake(1, dir, 0.1f);
	
	sound_mgr::instance().RandSoundKeyPlay("DASH", { 1,4 },1.f);

}

void Player::Attack()
{
	if (_player_info->bDash)return;
	if (!_transform)return;
	auto sp_Attack = NormalAttack.lock();
	if (!sp_Attack) return;
	sound_mgr::instance().RandSoundKeyPlay("NORMAL_ATTACK", { 1,3 }, 1.f);

	Anim& MyAnim = _render_component->_Anim;
	// 어택 듀레이션이 지난 다음에 어택을 풀어주기
	_player_info->bAttack = true;
	float DefaultAttackDuration = _player_info->DefaultAttackDuration;
	_player_info->CurrentAttackDuration = DefaultAttackDuration;

	if (!_transform) return;
	vec dis = *Input_mgr::instance().GetWorldMousePos() - _transform->_location;
	vec attack_dir = dis.get_normalize();

	// 어중간한 각도는 버린다. (애니메이션이 이상해보임)
	float fDegree = math::AngleFromVec(attack_dir);
	int iDegree = std::round(fDegree / 45.f);
	fDegree = iDegree * 45.f;

	vec Dir = math::dir_from_angle(fDegree);
	Dir.y *= -1;

	_transform->_dir = Dir;
	_transform->Move(_transform->_dir,_player_info->PlayerAttackMoveSpeed*DeltaTime);

	math::EDir _Dir = math::checkDir(attack_dir);

	switch (_Dir)
	{
	case math::EDir::left:
		_render_component->ChangeImg(AnimDirFileTable[(int)EAnimDir::left]);
		break;
	case math::EDir::right:
		_render_component->ChangeImg(AnimDirFileTable[(int)EAnimDir::right]);
		break;
	case math::EDir::up:
		_render_component->ChangeImg(AnimDirFileTable[(int)EAnimDir::front]);
		break;
	case math::EDir::down:
		_render_component->ChangeImg(AnimDirFileTable[(int)EAnimDir::back]);
		break;
	default:
		break;
	}

	_Shadow.CurrentShadowState = EShadowState::MIDDLE;

	AttackNumber  _ANum = AttackNumber::_1;

	if ((int)AnimTable::attack1 == MyAnim.CurClipRowIndex())
	{
		AttackNumber  _ANum = AttackNumber::_1;
		_render_component->ChangeAnim(AnimTable::attack2, _player_info->AttackAnimDuration);
	}
	else
	{
		AttackNumber  _ANum = AttackNumber::_2;
		_render_component->ChangeAnim(AnimTable::attack1, _player_info->AttackAnimDuration);
	};

	
	sp_Attack->AttackReady(_player_info->AttackCollisionDuration,
		_player_info->AttackAnimDuration, _player_info->NormalAttackPushForce,
		_transform->_location + _transform->_dir * _player_info->NormalAttackRich, _ANum,
		fDegree);

	/*vec dir{ math::Rand<float>({ -3,+3 }), math::Rand<float>({ -3,+3 }) };
	Camera_Shake(1, dir, 0.1f);*/
};

void Player::Player_Move(float dt)
{

	if (!_player_info)return;
	if (_player_info->bAttack)return;
	_player_info->bMove = false;
	if (!_player_info->bDash)
	{
		Input_mgr& _Input =Input_mgr::instance();
		if (_Input.Key_Pressing('D'))
		{
			_player_info->bMove = true;
			if (_Input.Key_Pressing('W'))
			{
				_Shadow.CurrentShadowState = EShadowState::MIDDLE;

				_render_component->ChangeAnim(AnimTable::walk, 0.5f, AnimTable::idle, AnimDirFileTable[(int)EAnimDir::back]);
				_player_info->bIdle = false;
				_transform->_dir = vec::unit_diagonal_vec();
				_transform->_dir.y *= -1;
				_transform->Move(_transform->_dir, _speed * dt);
			}
			else if (_Input.Key_Pressing('S'))
			{
				_Shadow.CurrentShadowState = EShadowState::MIDDLE;

				_render_component->ChangeAnim(AnimTable::walk, 0.5f, AnimTable::idle, AnimDirFileTable[(int)EAnimDir::front]);
				_transform->_dir = vec::unit_diagonal_vec();
				_player_info->bIdle = false;
				_transform->Move(_transform->_dir, _speed * dt);
			}
			else
			{
				_Shadow.CurrentShadowState = EShadowState::MIDDLE;
				_render_component->ChangeAnim(AnimTable::walk, 0.5f, AnimTable::idle, AnimDirFileTable[(int)EAnimDir::right]);
				_player_info->bIdle = false;
				_transform->_dir = vec{ +1,0 };
				_transform->Move(_transform->_dir, _speed * dt);
			}
		}
		else if (_Input.Key_Pressing('A'))
		{
			_player_info->bMove = true;
			if (_Input.Key_Pressing('W'))
			{
				_Shadow.CurrentShadowState = EShadowState::MIDDLE;

				_render_component->ChangeAnim(AnimTable::walk, 0.5f, AnimTable::idle, AnimDirFileTable[(int)EAnimDir::back]);
				_player_info->bIdle = false;
				_transform->_dir = vec::unit_diagonal_vec();
				_transform->_dir *= -1;
				_transform->Move(_transform->_dir, _speed * dt);
			}
			else if (_Input.Key_Pressing('S'))
			{
				_Shadow.CurrentShadowState = EShadowState::MIDDLE;

				_render_component->ChangeAnim(AnimTable::walk, 0.5f, AnimTable::idle, AnimDirFileTable[(int)EAnimDir::front]);
				_transform->_dir = vec::unit_diagonal_vec();
				_transform->_dir.x *= -1;
				_player_info->bIdle = false;
				_transform->Move(_transform->_dir, _speed * dt);
			}
			else
			{
				_Shadow.CurrentShadowState = EShadowState::MIDDLE;
				_render_component->ChangeAnim(AnimTable::walk, 0.5f, AnimTable::idle, AnimDirFileTable[(int)EAnimDir::left]);
				_player_info->bIdle = false;

				_transform->_dir = vec{ -1,0 };
				_transform->Move(_transform->_dir, _speed * dt);
			}
		}
		else if (_Input.Key_Pressing('W'))
		{
			_player_info->bMove = true;
			_Shadow.CurrentShadowState = EShadowState::MIDDLE;

			_render_component->ChangeAnim(AnimTable::walk, 0.5f, AnimTable::idle, AnimDirFileTable[(int)EAnimDir::back]);
			_player_info->bIdle = false;
			_transform->_dir = vec{ 0,-1 };
			_transform->Move(_transform->_dir, _speed * dt);
		}
		else if (_Input.Key_Pressing('S'))
		{
			_player_info->bMove = true;
			_Shadow.CurrentShadowState = EShadowState::MIDDLE;
			_player_info->bIdle = false;

			_render_component->ChangeAnim(AnimTable::walk, 0.5f, AnimTable::idle, AnimDirFileTable[(int)EAnimDir::front]);
			_transform->_dir = vec{ 0,+1 };
			_transform->Move(_transform->_dir, _speed * dt);
		}
	}

	

	if (_player_info->bMove)
	{
		sound_mgr::instance().RandSoundKeyPlay("RUN", { 1,4 }, 1.f);
	}
}

void Player::Die()
{
	sound_mgr::instance().Play("PLAYER_DIE", false, 1.f);
}

void Player::MissMsg()
{
	vec randvec = math::RandVec();
	randvec.y = (abs(randvec.y));
	vec v = _transform->_location;
	v.y -= 35;
	v.x += math::Rand<int>({ -40,+40 });

	object_mgr::instance().TextEffectMap[RGB(108, 32,200)].
		push_back({ v ,vec{0,1}*3,
		1.f,30,L"MISS" });

	collision_mgr::instance().HitEffectPush(_transform->_location, 0.5f);
}

