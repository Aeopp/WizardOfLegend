#pragma once
#include "actor.h"
#include "collision_component.h"
#include "EnemyInfo.h"
#include "Effect.h"

class Monster :
    public actor
{
public:
	enum class EMonsterState :uint8_t
	{
		Idle,
		Walk,
		Attack,
		Hit,
		Dead,
		Skill1,
		Skill2,
		Skill3,
		Skill4,
	};

	void CheckDirChangeImgFile();
	void initialize()override;
	Event update(float dt)override;
	void Hit(std::weak_ptr<object> _target)override;
	void late_initialize(std::weak_ptr<class object> SetTarget,
	vec SetLocation);
public:
	std::weak_ptr<collision_component>  _collision_component{};

	vec collision_lower_correction{};

	bool bDying = false;
	float DefaultHitDuration = 0.25f;
	bool bInvincible = false;
	float InvincibleTime = 0.1f;
	float StateDuration{};
	EMonsterState CurrentState{ EMonsterState::Idle };
	std::weak_ptr<class object> _AttackTarget;
	EnemyInfo _EnemyInfo{};
public:
	static void CardEffect(vec v,std::wstring ImageKey);
protected:
	// ����Ʈ ������ �� ����Ʈ �����ϵ� �ʱ�ȭ
	// ���ʹ� ���� ���鵵 �ʱ�ȭ
	std::wstring LeftAnimKey;
	std::wstring RightAnimKey;
	vec  lower_size,  lower_correction;
	std::pair<int,int>shadow_correction;
	std::vector<int> MyAnimInfo{};
	float MyAnimDuration;
	std::pair<int,int> ShadowWorldSizeCorrection;
	float InitTime;
};

