#pragma once

struct player_info
{
	float gold = { 0 };

	float max_hp{500};
	float max_mp{500};

	float  hp{500};
	float mp{500};

	float dash_speed{ 1000.f };

	bool bAttack{ false };
	bool bIdle{ true };
	bool bDash{ false };
	bool bMove{ false };
	float DashDuration = 0.4f;
	float DefaultAttackDuration = 0.3f;
	float CurrentAttackDuration = 0.3f;

	float SkillICECrystalMotionDuration = 1.0f;
	float SkillICEBlastMotionDuration = 1.0f;
	float SkillShieldMotionDuration = 1.0f;
};

struct PlayerAnimInfo
{
	
};
