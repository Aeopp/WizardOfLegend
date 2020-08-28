#pragma once
#include <utility>

struct EnemyInfo
{
	int HP{ 100 };
	std::pair<int, int> AttackRange{ 10,20 };
	float AttackStartDistance { 100.f };
};