#pragma once

struct player_info
{
	float gold = { 0 };

	float max_hp{500};
	float max_mp{500};

	float dash_speed{ 1100.f };

	bool bAttack{ false };
	bool bIdle{ true };
	bool bDash{ false };
	bool bMove{ false };
	bool bHit{ false };
	float DashDuration = 0.3;
	float AnimDashDuration = 0.3;
	float PlayerAttackMoveSpeed = 1500;
	float AttackCollisionDuration = 0.1f;
	float DefaultAttackDuration = 0.4;
	float AttackAnimDuration = 0.4;
	float CurrentAttackDuration = DefaultAttackDuration;

	float  	SkillBoomerangMotionDuration = 0.35f;
	float SkillFireDragonMotionDuration = 0.3f;
	float SkillICECrystalMotionDuration = 0.5f;
	float SkillICEBlastMotionDuration = 0.5f;
	float SkillShieldMotionDuration = 0.5f;

	const float SkillFireDragonMaxNum = 8; 
	const float SkillBoomerangMaxNum = 4;
	const float SkillICECrystalCoolTime = 6.5f;
	const float SkillICEBlastCoolTime = 3.5f;
	const float SkillShieldCoolTime = 13.f;

	float SkillCurrentICECrystalCoolTime = SkillICECrystalCoolTime;
	float SkillCurrentICEBlastCoolTime = SkillICEBlastCoolTime;
	float SkillCurrentShieldCoolTime = SkillShieldCoolTime;
	float SkillCurrentFireDragonNum = SkillFireDragonMaxNum;
	float SkillCurrentBoomerangNum = SkillBoomerangMaxNum;

	float MpRegenerationAtSec = -50.f;
	float HpRegenerationAtSec= 10.f;

	float ShieldDuration = 20.f;

	float NormalAttackRich{60.f};
	float NormalAttackPushForce{1.f};

	std::pair<int, int> MissRange{0,8};
	const float TeleportCoolTime = 5.f;
	float CurrentTeleportCoolTime = TeleportCoolTime; 
public : 
	void AddGold(int Gold);
	void AddHp(float HP); 
	void AddMp(float MP);
	void SetHp(float HP);
	void SetMp(float MP);
	float GetHP();
	float GetMP();
	void SignatureGaugeAdd(float Damage);
private:
	float  hp{ max_hp };
	float mp{ 0.f };
};
