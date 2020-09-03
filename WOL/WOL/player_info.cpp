#include "pch.h"
#include "player_info.h"
#include "sound_mgr.h"

void player_info::AddGold(int Gold)
{
	sound_mgr::instance().Play("MONEY_TAKE", false, 1.f);
	gold += Gold;
}

void player_info::AddHp(int32_t HP)
{
	hp = std::clamp<>(hp + HP, 0, max_hp);
}

void player_info::AddMp(int32_t MP)
{
	mp = std::clamp<>(mp + MP, 0, max_mp);
};

void player_info::SetHp(uint32_t HP)
{
	hp = HP;
}

void player_info::SetMp(uint32_t MP)
{
	mp = MP;
}

uint32_t player_info::GetHP()
{

	return hp;
}

uint32_t player_info::GetMP()
{
	return mp;
}
