#include "pch.h"
#include "player_info.h"
#include "sound_mgr.h"

void player_info::AddGold(int Gold)
{
	sound_mgr::instance().Play("MONEY_TAKE", false, 30.f);
	gold += Gold;
}

void player_info::AddHp(uint32_t HP)
{
	hp += HP;

	if (hp < 0)
	{
		hp = 0;
	}
}

void player_info::AddMp(uint32_t MP)
{
	mp+= MP;

	if (mp < 0)
	{
		mp = 0;
	}

}

uint32_t player_info::GetHP()
{

	return hp;
}

uint32_t player_info::GetMP()
{
	return mp;
}
