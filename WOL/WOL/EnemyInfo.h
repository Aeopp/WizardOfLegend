#pragma once
#include <utility>

struct EnemyInfo
{
	int HP{ 100 };
	std::pair<int, int> AttackRange{ 10,20 };
	float AttackStartDistance { 100.f };
	float DeadTimer = 1.f;

	bool bAttack = false;
	bool bHit = false;
};