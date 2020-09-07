#pragma once
#include "Monster.h"

class SwordMan :
 public Monster
{
public:
    void initialize()override;
    Event update(float dt)override;
    void Hit(std::weak_ptr<object> _target)override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;

    float StalkerPosReTargetDuration;
    float AttackEndRemainTime;
    float AttackStartTime;
    static inline const std::wstring SummonCardImgKey = L"SUMMON_CARD_SWORDMAN";

    std::weak_ptr<class SwordManAttack> NormalAttack{};

    void AttackReadyCheck();
    float SoundTick = 0.1f;
private:
    enum class EAnimState : uint8_t
    {
        Idle,
        Walk,
        Attack,
        Hit,
        Dead,
    };

    // Monster을(를) 통해 상속됨
    virtual std::wstring GetSummonKey() override;
};

