#include "pch.h"
#include "player.h"
#include "collision_mgr.h"

void player::initialize()
{
	auto ptr = collision_mgr::instance().insert(_ptr, collision_tag::player, ERect);
	ptr->_size = 30.0f;

}
