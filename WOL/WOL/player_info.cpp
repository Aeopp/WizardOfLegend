#include "pch.h"
#include "player_info.h"
#include "sound_mgr.h"

void player_info::AddGold(int Gold)
{
	sound_mgr::instance().Play("MONEY_TAKE", false, 1.f);
	gold += Gold;
}

void player_info::AddHp(float HP)
{
	hp = std::clamp<>(hp + HP, 0.f, max_hp);
}

void player_info::AddMp(float MP)
{
	mp = std::clamp<>(mp + MP, 0.f, max_mp);
};

void player_info::SetHp(float HP)
{
	hp = HP;
}

void player_info::SetMp(float MP)
{
	mp = MP;
}

float player_info::GetHP()
{

	return hp;
}

float player_info::GetMP()
{
	return mp;
}

void player_info::SignatureGaugeAdd(float Damage)
{
	AddMp(Damage * 1.f);
}
