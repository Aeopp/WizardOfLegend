#pragma once

struct player_info
{
	float gold = { 0 };

	float max_hp{1000};
	float max_mp{1000};

	float dash_speed{ 1700.f };

	bool bAttack{ false };
	bool bIdle{ true };
	bool bDash{ false };
	bool bMove{ false };
	bool bHit{ false };
	bool bProtected = false;
	float DashDuration = 0.3;
	float AnimDashDuration = 0.3;
	float PlayerAttackMoveSpeed = 3000;
	float AttackCollisionDuration = 0.1f;
	float DefaultAttackDuration = 0.4;
	float AttackAnimDuration = 0.4;
	float CurrentAttackDuration = DefaultAttackDuration;


	float SkillBoomerangMotionDuration = 0.35f;
	float SkillICECrystalMotionDuration = 0.5f;
	float SkillICEBlastMotionDuration = 0.5f;
	float SkillShieldMotionDuration = 0.5f;


	const float SkillBoomerangMaxNum = 8; 
	const float SkillICECrystalCoolTime = 6.5f;
	const float SkillICEBlastCoolTime = 3.5f;
	const float SkillShieldCoolTime = 13.f;

	float SkillCurrentICECrystalCoolTime = SkillICECrystalCoolTime;
	float SkillCurrentICEBlastCoolTime = SkillICEBlastCoolTime;
	float SkillCurrentShieldCoolTime = SkillShieldCoolTime;
	float SkillCurrentBoomerangNum = SkillBoomerangMaxNum;

	float SkillFireDragonMotionDuration = 0.2f;

	float MpRegenerationAtSec = 30.f;
	float HpRegenerationAtSec=50.f;

	float ShieldDuration = 20.f;

	float NormalAttackRich{60.f};
	float NormalAttackPushForce{5.f};



	std::pair<int, int> MissRange{0,8};
public : 
	void AddGold(int Gold);
	void AddHp(float HP); 
	void AddMp(float MP);
	void SetHp(float HP);
	void SetMp(float MP);
	float GetHP();
	float GetMP();
private:
	float  hp{ max_hp };
	float mp{ max_mp };
};

struct PlayerAnimInfo
{
	
};
