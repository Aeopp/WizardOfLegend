#pragma once

struct player_info
{
	float gold = { 0 };

	int32_t max_hp{1000};
	int32_t max_mp{1000};

	float dash_speed{ 1500.f };

	bool bAttack{ false };
	bool bIdle{ true };
	bool bDash{ false };
	bool bMove{ false };
	float DashDuration = 0.3;
	float DefaultAttackDuration = 0.5f;
	float CurrentAttackDuration = DefaultAttackDuration;

	float SkillBoomerangMotionDuration = 1.0f;
	float SkillICECrystalMotionDuration = 1.0f;
	float SkillICEBlastMotionDuration = 1.0f;
	float SkillShieldMotionDuration = 1.0f;
	float SkillFireDragonMotionDuration = 0.4f;
	float MpRegenerationAtSec = 30.f;
	float HpRegenerationAtSec=50.f;

	float NormalAttackRich{80.f};
	float NormalAttackPushForce{0.3f};
public : 
	void AddGold(int Gold);
	void AddHp(int32_t HP); 
	void AddMp(int32_t MP);
	void SetHp(uint32_t HP);
	void SetMp(uint32_t MP);
	uint32_t GetHP();
	uint32_t GetMP();
private:
	int32_t  hp{ max_hp };
	int32_t mp{ max_mp };
};

struct PlayerAnimInfo
{
	
};
