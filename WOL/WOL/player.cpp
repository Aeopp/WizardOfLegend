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
#include "ICE_Crystal.h"
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



void Player::render(HDC hdc, vec camera_pos, vec size_factor)
{
	actor::render(hdc, camera_pos, size_factor);
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

	//Anim SetUp
	{
		_render_component = render_component::LoadRenderComponent_SP
		(L"FRONT_COMPLETE.bmp", L"FRONT_COMPLETE");
		// 174 182
		_render_component->Default_Dest_Paint_Size = vec{ 182,182 };
		_render_component->Dest_Paint_Size = vec{ 182*0.8,182*0.8 };
		_render_component->_ColorKey = COLOR::MEGENTA();
		_render_component->_Img_src = RECT{ 0,0,174,182 };
		_render_component->_Anim.SetAnimationClip(
			{ 1 ,10,8,8,8,2,7 }, 0.05f);
	}

	make_skill_bar();

	make_gold_UI();

	make_player_bar();

	make_skillbar_icon(ESkill::ARMOR);
	make_skillbar_icon(ESkill::BLAST);
	make_skillbar_icon(ESkill::CRYSTAL);
	make_skillbar_icon(ESkill::FIRE);

	_speed = 400.f;

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
		MyAnim.AnimPlay((int)AnimTable::hit,0.3f);
		MyAnim.SetDefaultClip((int)AnimTable::idle);
	}
	else
	{
		_render_component->wp_Image = AnimDirFileTable[(int)EAnimDir::front];
		MyAnim.AnimPlay((int)AnimTable::dead, 0.3f);
		MyAnim.SetDefaultClip((int)AnimTable::idle);
	}
};

void Player::temp(float temp)
{
	MessageBox(game::instance().hWnd, __FUNCTIONW__, __FUNCTIONW__, MB_OK);
}

void Player::MakeShield()
{
	Input_mgr& _Input = Input_mgr::instance();

	static float shield_distance = 150.0f;
	static float shield_speed = 360.f;

	object_mgr& _obj_mgr = object_mgr::instance();

	float degree= 360.0f / 8.f;

	for (int i = 0; i < 8; ++i)
	{
		auto _shield = _obj_mgr.insert_object<shield>();

	   _shield->_transform->_dir = math::dir_from_angle(degree * i);

		if (!_shield) return;
		_shield->_owner = _ptr;

		_shield->_transform->_location = _transform->_location + _shield->_transform->_dir * shield_distance;

		_shield->_shield_distance = shield_distance;
		_shield->_speed = shield_speed;
	}
}

void Player::ICE_CRYSTAL()
{
	Input_mgr& _Input = Input_mgr::instance();

	auto V= _Input.GetWorldMousePos();

	if (!V) return;

	auto _ICE = object_mgr::instance().insert_object<ICE_Crystal>();

	if (!_ICE) return;

	_ICE->_owner = _ptr;
	_ICE->_target = *V;
	_ICE->_transform->_location = _transform->_location;

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

	if (_Input.Key_Pressing(VK_RIGHT))
	{
		_render_component->ChangeAnim(AnimTable::walk,0.2f, AnimTable::idle, AnimDirFileTable[(int)EAnimDir::right]);

		_transform->_dir = vec{ +1,0 };
		_transform->Move(_speed * dt);
	}
	else if (_Input.Key_Pressing(VK_LEFT))
	{
		_render_component->ChangeAnim(AnimTable::walk, 0.2f, AnimTable::idle, AnimDirFileTable[(int)EAnimDir::left]);

		_transform->_dir = vec{ -1,0 };
		_transform->Move(_speed * dt);
	}
	else if (_Input.Key_Pressing(VK_UP))
	{
		_render_component->ChangeAnim(AnimTable::walk, 0.2f, AnimTable::idle,AnimDirFileTable[(int)EAnimDir::back]);

		_transform->_dir = vec{ 0,-1 };
		_transform->Move(_speed * dt);
	}
	else if (_Input.Key_Pressing(VK_DOWN))
	{
		_render_component->ChangeAnim(AnimTable::walk, 0.2f, AnimTable::idle, AnimDirFileTable[(int)EAnimDir::front]);
		_transform->_dir = vec{ 0,+1 };
		_transform->Move(_speed * dt);
	};


	if (_Input.Key_Down('W'))
	{
		ICE_CRYSTAL();
	}

	if (_Input.Key_Down(VK_LBUTTON))
	{
		Anim& MyAnim = _render_component->_Anim;

		if ((int)AnimTable::attack1 == MyAnim.CurClipRowIndex())
		{
			_render_component->ChangeAnim(AnimTable::attack2, 0.2f, AnimTable::idle);
		}
		else 
		{
			_render_component->ChangeAnim(AnimTable::attack1, 0.2f, AnimTable::idle);
		}

		Camera_Shake(5, vec{ 1,1 }, 0.1f);
	}

	if (_Input.Key_Down('Q'))
	{
		MakeShield();
	}

	if (_Input.Key_Down(VK_SPACE))
	{
		Dash();
	}
};

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
			vec{ 315+57,838 }, L"GAIA_ARMOR_SKILLBAR.bmp");
		break;
	default:
		break;
	}
}

void Player::Dash()
{
	_render_component->ChangeAnim(AnimTable::dash, 0.2f, AnimTable::idle);
};


