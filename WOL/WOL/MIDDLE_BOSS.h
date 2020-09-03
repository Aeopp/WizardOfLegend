#pragma once
#include "object.h"
#include "Shadow.h"

class MIDDLE_BOSS : public object
{
public:
    enum class EState : uint8_t
    {
        IDLE,
        SKILL1,
        SKILL2,
        SKILL3,
        HIT,
    };

    void initialize()override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    Event update(float dt)override;
    void Hit(std::weak_ptr<object> _target)override;

    //left right 스프라이트
    std::pair< std::shared_ptr<class Bmp> , std::shared_ptr<class Bmp> > sp_Bmp;
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
    static inline float AnimDelta = 0.15f;
    vec AttackDir{};

    float CurrentAnimDelta = AnimDelta;
    float AttackStartDistance;
    float StateDuration;
    float HitCoolTime = 0.1f;

    void AnimColUpdate(bool Updown);
    void AnimUpdateFromCurrentState();
    int AnimRowCalcFromAngle();
    void StateTranslation();
    void AttackStart();
    void StateAction();
    void UpdateDir();
    void ConditionCheck();
    EState CurrentState = EState::IDLE;
    bool Attacking();
    bool AttackEnd();
    void AttackReady();
    void IdleAction();
    bool bAttackEnd = false;

    int HP = 200;
};

