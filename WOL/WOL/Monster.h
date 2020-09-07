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
	void HitSoundPlayBackByTag(EObjUniqueID CheckID, object::Tag CheckTag);
	void DieSoundPlay();
	void MonsterDie();
	void ICEHitSoundPlay();
	void WINDHitSoundPlay();
	void NormalHitSoundPlay();
	void BoomerangSoundPlay();

	virtual std::wstring GetSummonKey()abstract;
public:
	std::weak_ptr<collision_component>  _collision_component{};

	vec collision_lower_correction{};

	bool bDying = false;
	float DefaultHitDuration = 0.4f;
	bool bInvincible = false;
	float InvincibleTime = 0.3f;
	float StateDuration{};
	std::weak_ptr<class object> _AttackTarget;
	EnemyInfo _EnemyInfo{};
public:
	static void CardEffect(vec v,std::wstring ImageKey);
protected:
	// 페인트 사이즈 랑 페인트 스케일도 초기화
	// 에너미 인포 값들도 초기화
	std::wstring LeftAnimKey;
	std::wstring RightAnimKey;
	vec  lower_size,  lower_correction;
	std::pair<int,int>shadow_correction;
	std::vector<int> MyAnimInfo{};
	float MyAnimDuration;
	std::pair<int,int> ShadowWorldSizeCorrection;
	float InitTime;
	
};

