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

	sp_collision->_size = { 30.f,30.0f };

	_Camera = object_mgr::instance()._Camera;

	_player_info = game::instance()._player_info;
	if (!_player_info)return;

	PaintSizeX = 180;
	PaintSizeY = 182;

	float Scale = 0.7f;

	//Anim SetUp
	{
		_render_component = render_component::LoadRenderComponent_SP
		(L"FRONT_COMPLETE.bmp", L"FRONT_COMPLETE");
		// 174 182
		_render_component->Default_Src_Paint_Size = vec{ PaintSizeX,PaintSizeY };
		_render_component->Dest_Paint_Size = vec{ PaintSizeX * Scale,PaintSizeY * Scale };
		_render_component->_ColorKey = COLOR::MEGENTA();
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

	_Shadow.correction = { 0,(PaintSizeY * 0.47) /2.f };


	Timer::instance().event_regist(time_event::ERemaingWhile, FLT_MAX,
		[&bAttack = _player_info->bAttack,&AttackCheck = _player_info->CurrentAttackDuration]()->bool{
		AttackCheck -= DeltaTime;
		if (AttackCheck < 0)bAttack = false;
		else  bAttack = true;

		return true;
	});
};

Event Player::update(float dt)
{
	Event _E = object::update(dt);

	player_check(dt);

	if (!_player_info) return Event::Die;
	_player_info->hp -= dt * 1;
	_player_info->mp -= dt * 10;


	return _E;
}
void Player::Hit(std::weak_ptr<object> _target)
{
	Anim& MyAnim = _render_component->_Anim;

	if(_player_info->hp>0)
	{
		_Shadow.CurrentShadowState = EShadowState::NORMAL;
		MyAnim.AnimPlay((int)AnimTable::hit,0.3f);
		MyAnim.SetDefaultClip((int)AnimTable::idle);
	}
	else
	{
		_Shadow.CurrentShadowState = EShadowState::NORMAL;
		_render_component->wp_Image = AnimDirFileTable[(int)EAnimDir::front];
		MyAnim.AnimPlay((int)AnimTable::dead, 0.3f);
		MyAnim.SetDefaultClip((int)AnimTable::idle);
	}
};

void Player::StateCheck()
{

}

void Player::temp(float temp)
{
	MessageBox(game::instance().hWnd, __FUNCTIONW__, __FUNCTIONW__, MB_OK);
}

void Player::MakeShield()
{
	Input_mgr& _Input = Input_mgr::instance();


	object_mgr& _obj_mgr = object_mgr::instance();

	float degree= 360.0f / 8.f;
	
	for (int i = 0; i < 8; ++i)
	{
		auto _shield = _obj_mgr.insert_object<shield>();

	   _shield->_transform->_dir = math::dir_from_angle(degree * i);

		if (!_shield) return;
		_shield->_owner = _ptr;

		_shield->HoleLocation = _shield->_transform->_location = _transform->_location + _shield->_transform->_dir * _shield->_shield_distance;
		_shield->Angle = degree * i;
		_shield->CalcIdx();
	}
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
	Camera_Shake(15, dir, 0.3f);


	
}

void Player::ICE_BLAST()
{
	if (!_player_info)return;
	if (_player_info->bDash)return;

	Input_mgr& _Input = Input_mgr::instance();

	auto V= _Input.GetWorldMousePos();

	if (!V) return;

	auto _ICE = object_mgr::instance().insert_object<ICE_Blast>();

	if (!_ICE) return;

	_player_info->bIdle = false;

	_ICE->_owner = _ptr;
	_ICE->_target = *V;
	_ICE->_transform->_location = _transform->_location;
	_Shadow.CurrentShadowState = EShadowState::BIG;

	Anim& MyAnim = _render_component->_Anim;
	_player_info->bAttack = true;
	_player_info->CurrentAttackDuration = _player_info->DefaultAttackDuration;


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

void Player::player_check(float dt)
{
	Input_mgr& _Input = Input_mgr::instance();

	_player_info->bIdle = true;

	if (!_player_info)return;

	Player_Move(dt);

	if (_Input.Key_Down('Q'))
	{

		ICE_BLAST();
	}

	if (_Input.Key_Down(VK_LBUTTON))
	{
		Attack();
	}

	if (_Input.Key_Down('R'))
	{

		MakeShield();
	}

	if (_Input.Key_Down('E'))
	{
		
		SkillIceCrystal(math::Rand<int>({ 1,12 }));
	}

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

void Player::SkillIceCrystal(uint32_t Num)
{
	if (!_player_info)return;
	if (_player_info->bDash)return;

	object_mgr& _object_mgr = object_mgr::instance();

	float degree = 360.0f / Num;

	for (int i = 0; i < 8; ++i)
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
	Camera_Shake(10, dir, 0.5f);
};

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

	HBar->current = _player_info->hp;
	HBar->goal_time = 0.5f;
	HBar->max = _player_info->hp;
	HBar->wp_info = _player_info;

	auto MBar = object_mgr::instance().insert_object<UIMPBar>();

	MBar->current = _player_info->mp;
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
		break;
	case ESkill::BLAST:
		USBI = object_mgr::instance().insert_object<UISkillIBarIcon>(
			vec{ 259,838 }, L"ICE_BLAST_SKILLBAR.bmp");
		break;
	case ESkill::CRYSTAL:
		USBI = object_mgr::instance().insert_object<UISkillIBarIcon>(
			vec{ 315,838 }, L"ICE_KRYSTAL_SKILLBAR.bmp");
		break;
	case ESkill::ARMOR:
		USBI = object_mgr::instance().insert_object<UISkillIBarIcon>(
			vec{ 315 + 57,838 }, L"GAIA_ARMOR_SKILLBAR.bmp");
		break;
	default:
		break;
	}
};

void Player::Dash(float speed)
{
	if (!_player_info)return;
	if (_player_info->bDash)return;

	_player_info->bDash = true;
	// 대쉬전에 방향 다시금 설정
	CheckDirInput();
	_player_info->bIdle = false;
	_Shadow.CurrentShadowState = EShadowState::BIG;

	Timer::instance().event_regist(time_event::EOnce, _player_info->DashDuration,
		[&bDash = _player_info->bDash]()->bool{
		bDash = false; return true; });
	_player_info->DashDuration;

	Timer::instance().event_regist(time_event::ERemaingWhile, _player_info->DashDuration,
		std::move([Transform=this->_transform,speed]()->bool {
		if (!Transform)return false;

		Transform->_location += Transform->_dir *(speed*DeltaTime);

		return true;
	}));
	_render_component->ChangeUnstoppableAnim(AnimTable::dash, _player_info->DashDuration, AnimTable::idle);

	vec dir{ math::Rand<float>({ -3,+3 }), math::Rand<float>({ -3,+3 }) };
	Camera_Shake(5, dir, 0.1f);
}

void Player::Attack()
{
	if (_player_info->bDash)return;
	Anim& MyAnim = _render_component->_Anim;
	// 어택 듀레이션이 지난 다음에 어택을 풀어주기
	_player_info->bAttack = true;
	_player_info->CurrentAttackDuration = _player_info->DefaultAttackDuration;

	_Shadow.CurrentShadowState = EShadowState::MIDDLE;

	if ((int)AnimTable::attack1 == MyAnim.CurClipRowIndex())
	{
		_render_component->ChangeAnim(AnimTable::attack2, _player_info->DefaultAttackDuration);
	}
	else
	{
		_render_component->ChangeAnim(AnimTable::attack1, _player_info->DefaultAttackDuration);
	};

	vec dir{ math::Rand<float>({ -3,+3 }), math::Rand<float>({ -3,+3 }) };
	Camera_Shake(5, dir, 0.1f);
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
}

