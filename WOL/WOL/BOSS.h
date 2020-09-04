#pragma once
#include "object.h"
#include "Shadow.h"

class BOSS : public object
{
public:
    enum class EState : uint8_t
    {
        IDLE,
        WALK,
        BOXSPAWN,
        BOXATTACK,
        PillarAttack,
        JUMPATTACK,
        HIT,
        DIE,
    };

    void initialize()override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    Event update(float dt)override;
    void Hit(std::weak_ptr<object> _target)override;
    void SetUp(std::weak_ptr<class object> AttackTarget, vec Location);
    virtual ~BOSS()noexcept;


    //left right 스프라이트
    std::pair< std::shared_ptr<class Bmp>, std::shared_ptr<class Bmp> > sp_Bmps;
    std::shared_ptr<class Bmp> sp_AttackBmp;

    std::weak_ptr<class collision_component> wp_collision;
    std::weak_ptr<class object> wp_AttackTarget;
    std::shared_ptr<class BossInfo> sp_MyInfo{};
    std::weak_ptr<class UIBossHPBar> wp_UIBossHPBar{};

    std::weak_ptr<class UIBossName> wp_UIBossName{};

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

    float CurrentAnimDelta = AnimDelta;
    float AttackStartDistance;
    float StateDuration;
    float CurrentHitCoolTime = 0.1f;
    static inline float DefaultHitCoolTime = 0.3f;

    std::function<void()> CurrentSKILL;
    // Left Right 스프라이트 테이블
    std::vector<uint32_t> CurrentAnimColNumTalble{ 1,9,2,8,2,1 };
    // Attack 스프라이트 테이블
    std::vector<uint32_t> CurrentAttackAnimColNumTable{ 4,4,4,4 };
    bool bAttackAnimSprite = false;

    int AnimAttackSpriteRowUpdateFromDir();
    std::shared_ptr<class Bmp> AnimDirSpriteUpdate();

    void HitCalc(std::pair<int, int> AttackRange);
    void AttackStart();
    void AnimColUpdate();
    void AnimUpdateFromCurrentState();
    void StateTranslation();
    void StateAction();
    void UpdateDir();
    void StateSetUp(EState NewState, float Duration, float StateAnimDelta);
    void ConditionCheck();
    EState CurrentState = EState::IDLE;
    void IdleAction();
    void DieAction();
};

