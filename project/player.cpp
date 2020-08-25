#include "pch.h"
#include "player.h"
#include "shield.h"
#include "game.h"
#include "barrel.h"

static void player_move(player*const _player)
{
	_player->set_direction(_player->get_barrel()->get_direction());
	_player->moveable_object::move();
};

static void make_shield(player* const _player)
{
	for (size_t i = 0; i < 4; ++i)
	{
		auto _shield = game::instance().insert_object<shield>();
		_shield->set_owner(_player->get_ptr().lock());
		_shield->set_angle({ 360.0f * (i + 1) / 4 });
	}
};

void player::update()
{
	super::update();

	 if (GetAsyncKeyState(VK_UP))
	{
		 player_move(this);
	}
	 if (GetAsyncKeyState(VK_DOWN))
	{
		 player_move(this);
	}
	 if (GetAsyncKeyState('S'))
	 {
		 make_shield(this);
	 }
	 if (GetAsyncKeyState(VK_SPACE))
	{
		this->fire();
	}
	 if (GetAsyncKeyState('A'))
	 {
		 if (_barrel)
		 {
			 _barrel->rotation_fire();
		 }
	 }
};
 void player::render(HDC hdc)
{
	 super::render(hdc);
};
 void player::initialize()
 {
	 super::initialize();

	 object::set_location({ 500,500 });
	 object::set_size({ 100,100 });
	 moveable_object::set_speed(1.5f);

	 _barrel = game::instance().insert_object<barrel>();
	 _barrel->set_owner(get_ptr().lock());
 };

void player::release()
{
	super::release();
}

void player::fire()
{
	if (_barrel)
	{
		_barrel->fire();
	}
}

