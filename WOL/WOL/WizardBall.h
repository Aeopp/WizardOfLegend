#pragma once
#include "object.h"
#include "Shadow.h"

class WizardBall :
    public object
{
public:
    enum class EState: uint8_t
    {
        BALL,
        AttackReady,
        Attacking,
        AttackEnd,
        HIT,
    };
    void initialize()override;
    void late_initialize(std::weak_ptr<class object> wp_AttackTarget, vec location);
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    Event update(float dt)override;
    void Hit(std::weak_ptr<object> _target)override;
    static inline const std::wstring SummonCardImgKey = L"SUMMON_CARD_WIZARDBALL";

    std::shared_ptr<class Bmp> sp_Bmp;
    std::weak_ptr<class collision_component> wp_collision;
    std::weak_ptr<class object> wp_AttackTarget;
    Shadow _Shadow;

    int PaintSizeX;
    int PaintSizeY;
    float ScaleX;
    float ScaleY;
    int CurrentRowIdx = 0;
    int CurrentColIdx = 0;
    float AttackSpeed;
    float Speed;
    float InitTime = 3.5f;
    bool bDef = false;
    static inline float AnimDelta = 0.15f;
    vec AttackDir{};

    float CurrentAnimDelta = AnimDelta; 
    float AttackStartDistance;
    float StateDuration;
    float CurrentHitCoolTime = 0.1f;

    void AnimColUpdate(bool Updown);
    void AnimUpdateFromCurrentState();
    int AnimRowCalcFromAngle();
    void StateTranslation();
    void AttackStart();
    void StateAction();
    void UpdateDir();
    void ConditionCheck();
    EState CurrentState = EState::BALL;
    bool Attacking();
    bool AttackEnd();
    void AttackReady();
    void IdleAction();
    bool bAttackEnd = false;
    int HP = 200;

};

