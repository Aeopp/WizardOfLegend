#pragma once
#include "object.h"
#include "Shadow.h"
#include "Burning_Interface.h"

class MIDDLE_BOSS : public object , public Freezing_Interface , public Burning_Interface
{
public:
    enum class EState : uint8_t
    {
        IDLE,
        WALK,
        READY,
        CAST,
        HIT,
        DIE,
    };
    enum class EPattern : uint8_t
    {
        FIRE,
        BLAST,
        CRYSTAL,
        BOOMERANG,
        FIRE_DRAGON,
    };
    virtual ~MIDDLE_BOSS()noexcept;

    void initialize()override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    Event update(float dt)override;
    void Hit(std::weak_ptr<object> _target)override;
    void SetUp(std::weak_ptr<class object> AttackTarget,vec Location);

    //left right 스프라이트
    std::pair< std::shared_ptr<class Bmp> , std::shared_ptr<class Bmp> > sp_Bmps;
    std::weak_ptr<class collision_component> wp_collision;
    std::weak_ptr<class object> wp_AttackTarget;
    std::shared_ptr<class BossInfo> sp_MyInfo{};
    std::shared_ptr<class Bmp> FireImg{};
    std::weak_ptr<class UIBossName> wp_UIBossName{};
    std::weak_ptr<class UIBossHPBar> wp_UIBossHPBar{};


    Shadow _Shadow;

    int PaintSizeX;
    int PaintSizeY;
    float ScaleX;
    float ScaleY;
    int CurrentRowIdx = 0;
    int CurrentColIdx = 0;
    float Speed;
    static inline float AnimDelta = 0.15f;
    vec AttackDir{};
    vec MoveMark{};
    vec TargetLocation{};

    float CurrentAnimDelta = AnimDelta;
    float AttackStartDistance;
    float StateDuration;
    float CurrentHitCoolTime = 0.1f;
    const float MonsterSpawnCycle = 30.f;
    float MonsterSpawnCurrentTick = MonsterSpawnCycle ;

    static inline float DefaultHitCoolTime = 0.3f; 

    float BallSpawnSkillCurrentAngle = 0.f;

    std::function<void()> CurrentSKILL;
    std::vector<uint32_t> CurrentAnimColNumTalble{ 1,4,4,4,2,4 };
    std::shared_ptr<class Bmp> AnimDirSpriteUpdate();


	 std::vector<EPattern> PatternTable
	 { EPattern::FIRE,EPattern::BLAST,EPattern::FIRE_DRAGON,EPattern::CRYSTAL,EPattern::BOOMERANG,
	  EPattern::FIRE_DRAGON,EPattern::CRYSTAL,EPattern::FIRE,EPattern::BLAST,EPattern::BOOMERANG,
	 EPattern::FIRE_DRAGON,EPattern::CRYSTAL,EPattern::FIRE,EPattern::BLAST,EPattern::FIRE };

    int PatternTableNum;
    int CurrentPatternIdx = 0;

    bool bDieAction = true;

    void HitCalc(std::pair<int, int> AttackRange);
    void ReadyAction();
    void CastAction();
    void MoveMarkReNew();
    void WalkAction();
    void AttackStart();
    void AnimColUpdate();
    void AnimUpdateFromCurrentState();
    void StateTranslation();
    void StateAction();
    void UpdateDir();
    void StateSetUp(EState NewState, float Duration);
    void ConditionCheck();
    EState CurrentState = EState::IDLE;
    void IdleAction();
    // // //////////////////////////////////////
    void BOSS_Skill_Fire();
    void BOSS_SKill_BallSpawn();
    // 전방위 각으로 몇개나 발사할지
    void BOSS_SKill_HomingBlast();
    void BOSS_Skill_ICECrystal(size_t NUM);
    void BOSS_SKill_FireDragon(size_t NUM);
    void BOSS_SKill_Boomerang(size_t NUM);
    void BOSS_SKill_Blast();
     // // //////////////////////////////////////
    void BOSS_Blast_launch(size_t NUM, vec Dir);
    void FireSpawn();
    void BallSpawn(size_t NUM);
    void SummonMonster();
    void DieAction();
    // 전방위 각으로 발사할 때의 각을 나누는 개수
    void BlastSpawn(size_t NUM);
};

