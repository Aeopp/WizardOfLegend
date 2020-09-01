#pragma once

struct player_info
{
	float gold = { 0 };

	float max_hp{500};
	float max_mp{500};

	float dash_speed{ 1500.f };

	bool bAttack{ false };
	bool bIdle{ true };
	bool bDash{ false };
	bool bMove{ false };
	float DashDuration = 0.3f;
	float DefaultAttackDuration = 0.2f;
	float CurrentAttackDuration = 0.2f;

	float SkillICECrystalMotionDuration = 1.0f;
	float SkillICEBlastMotionDuration = 1.0f;
	float SkillShieldMotionDuration = 1.0f;
	float SkillFireDragonMotionDuration = 0.4f;

	float NormalAttackRich{80.f};
	float NormalAttackPushForce{0.3f};
public : 
	void AddGold(int Gold);
	void AddHp(uint32_t HP); 
	void AddMp(uint32_t MP);
	uint32_t GetHP();
	uint32_t GetMP();
private:
	uint32_t  hp{ 500 };
	uint32_t mp{ 500 };
};

struct PlayerAnimInfo
{
	
};
