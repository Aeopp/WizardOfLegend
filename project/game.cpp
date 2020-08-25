#include "pch.h"
#include "game.h"
#include "object.h"
#include "player.h"
#include "shield.h"
#include "stalker.h"
#include "collision_manager.h"
#include "math.h"


void game::render(HDC hdc)
{
	for (auto& objects : object_list)
	{
		for (auto& _object : objects.second)
		{
			_object->render(hdc);
		}
	}
};

void game::update()
{
	for (auto& objects : object_list)
	{
		for (auto& _object : objects.second)
		{	
			_object->update();
		}
	};
}

void game::initialize()
{
	collision_manager::instance().initialize();

	auto _player = insert_object<player>();

	auto _monster = insert_object<stalker>();
	_monster->set_size({ 75.0f , 75.0f });
	_monster->set_location({ 0,0 });
	_monster->set_target(_player->get_ptr());

	_monster = insert_object<stalker>();
	_monster->set_size({75.0f,75.0f });
	_monster->set_location({1200,0});
	_monster->set_target(_player->get_ptr());

	_monster = insert_object<stalker>();
	_monster->set_size({ 75.0f,75.0f });
	_monster->set_location({ 600,0 });
	_monster->set_target(_player->get_ptr());

	_monster = insert_object<stalker>();
	_monster->set_size({ 75.0f,75.0f });
	_monster->set_location({ 0,1500 });
	_monster->set_target(_player->get_ptr());

	_monster = insert_object<stalker>();
	_monster->set_size({ 75.0f,75.0f });
	_monster->set_location({ 1600,1600});
	_monster->set_target(_player->get_ptr());
}

void game::release()
{
	for (auto& objects : object_list)
	{
		for (auto& _object : objects.second)
		{
			_object->release();
		}
	}
}
