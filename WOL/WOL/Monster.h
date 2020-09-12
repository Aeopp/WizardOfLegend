#pragma once
#include "actor.h"
#include "collision_component.h"
#include "EnemyInfo.h"
#include "Effect.h"
#include "Freezing_Interface.h"
#include "Burning_Interface.h"
#include "object_mgr.h"


class Monster :  public actor ,  public Freezing_Interface , public Burning_Interface 
{
public:
	template<typename Monster_Type>
	static std::vector<std::weak_ptr<class object>> TypeMatchMonstersSpawn
	(std::weak_ptr<class object> _Target,std::vector<vec> MonsterLocationVec);
	
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

	void render(HDC hdc, vec camera_pos, vec size_factor) override;
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

	void Hit_Calculation(int HitAnimIdx, std::pair<int, int> AttackRange);
	void burn();
	static void MonsterHitPlayerSignatureGageAdd(float Atk);
	virtual std::wstring GetSummonKey()abstract;
public:
	std::weak_ptr<collision_component>  _collision_component{};

	vec collision_lower_correction{};
	bool bDying = false;
	float HitCoolTime = 0.25f;
	bool bInvincible = false;
	float InvincibleTime = HitCoolTime;
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

/// <summary>
/// 
/// </summary>
/// <typeparam name="Monster_Type"></typeparam>
/// Monster를 베이스로하는 하위 클래스
/// <param name="_Target"></param>
/// 생존하는 동안 공격할 타겟
/// <param name="MonsterLocationVec"></param>
/// 해당 타입의 소환 위치 목록
/// <returns></returns>
template <typename Monster_Type>
std::vector<std::weak_ptr<class object>> Monster::TypeMatchMonstersSpawn(
	std::weak_ptr<object> _Target,
	std::vector<vec> MonsterLocationVec)
{
	static_assert(std::is_base_of_v<Monster, Monster_Type>, 
	L"It is not a class with Monster as a Super class.");
	if (MonsterLocationVec.empty())return {};
	
	size_t MonsterNum = MonsterLocationVec.size();
	
	std::vector<std::weak_ptr<class object>> ReturnMonsters;

	ReturnMonsters.reserve(MonsterNum);
	
	auto Insert_Monster_Iter = std::back_inserter(ReturnMonsters);
	
	for(auto& SpawnLocation : MonsterLocationVec)
	{
		auto sp_Monster = object_mgr::instance().insert_object< Monster_Type>
		( _Target, std::move ( SpawnLocation  ) );
		
		if (!sp_Monster)continue;

		Insert_Monster_Iter = sp_Monster;
	}
	
	return ReturnMonsters;
}
