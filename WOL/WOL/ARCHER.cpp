#include "pch.h"
#include "ARCHER.h"


#include "Bmp_mgr.h"
#include "collision_mgr.h"
#include "render_component.h"
#include "Color.h"
#include "timer.h"

void ARCHER::initialize()
{
	Monster::initialize();
	Bmp_mgr& BMgr = Bmp_mgr::instance();

	AnimDirFileTable[(int)EAnimDir::left] = BMgr.Insert_Bmp(L"ARCHER_LEFT.bmp", L"ARCHER_LEFT");
	AnimDirFileTable[(int)EAnimDir::right] = BMgr.Insert_Bmp(L"ARCHER_RIGHT.bmp", L"ARCHER_RIGHT");

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EMonster, ERect);
	auto sp_collision = _collision_component.lock();

	if (!sp_collision)return;

	sp_collision->_size = { 30.f,30.0f };

	//Anim SetUp
	{
		_render_component = render_component::LoadRenderComponent_SP
		(L"ARCHER_RIGHT.bmp", L"ARCHER_RIGHT");
		// 200 216
		vec _ImageDefaultSize{ 200,200 };

		_render_component->Default_Dest_Paint_Size = _ImageDefaultSize;
		_render_component->Dest_Paint_Size = _ImageDefaultSize * 0.8;
		_render_component->_ColorKey = COLOR::MEGENTA();
		_render_component->_Img_src = RECT{ 0,0,200,200 };
		_render_component->_Anim.SetAnimationClip(
			{ 1,5,4,2,6 }, 0.3f);
	};
}

Event ARCHER::update(float dt)
{
	Event _E = Monster::update(dt);

	auto sp_AttackTarget = _AttackTarget.lock();
	if (!sp_AttackTarget) return Event::None;

	vec target_loc = sp_AttackTarget->_transform->_location;
	vec my_loc = _transform->_location;

	vec to_target = target_loc - my_loc;
	vec to_dir = to_target.get_normalize();

	float cur_distance = to_target.length();

	_render_component->ChangeAnim(AnimTable::walk, 0.3f, AnimTable::idle);

	_transform->_dir = to_dir;
	_transform->Move(_speed * dt);

	if (!_EnemyInfo)return Event::None;

	float start_dis = _EnemyInfo->AttackStartDistance;

	if (cur_distance < start_dis)
	{
		Attack();
	}

	vec dir = _transform->_dir;

	if (dir.x > 0)
	{
		_render_component->wp_Image = AnimDirFileTable[(int)EAnimDir::right];
	}
	else
	{
		_render_component->wp_Image = AnimDirFileTable[(int)EAnimDir::left];
	}

	return _E;
}

void ARCHER::Hit(std::weak_ptr<object> _target)
{
	Monster::Hit(_target);

	Anim& MyAnim = _render_component->_Anim;

	if (!_EnemyInfo) return;

	if (_EnemyInfo->HP > 0)
	{
		_render_component->ChangeAnim(AnimTable::hit, 0.2f, AnimTable::idle);
	}
	else
	{
		_render_component->ChangeAnim(AnimTable::dead, 0.4f, AnimTable::dead);

		Timer::instance().event_regist(time_event::EOnce, 0.4f, [&_Die = this->bDie]
			{
				_Die = true;
				return true;
			});
	};
}

void ARCHER::Attack()
{
	_render_component->ChangeAnim(AnimTable::attack, 0.3f, AnimTable::idle);
}