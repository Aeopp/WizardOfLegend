#pragma once
#include "Monster.h"

class WIZARD :
    public Monster
{
public:
    void initialize()override;
    Event update(float dt)override;
    void Hit(std::weak_ptr<object> _target)override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;

    static inline const std::wstring SummonCardImgKey = L"SUMMON_CARD_WIZARD";

    static inline float FireCoolTime = 0.5f;
    static inline float InitBallCoolTime = 1.f;
    static inline float BallCoolTime = 25.f;

    float CurrentFireCoolTime = FireCoolTime; 
    float CurrentBallCoolTime = InitBallCoolTime;

    float AttackPreparation = 1.f;
    vec EscapeVec{};
    float EscapeDuration;
    float EscapeRamainTick;
    float StalkerPosReTargetDuration;
    vec RandMoveVec{};
    static inline float RandMoveVecDuration = 4.f;
    float CurrentRandMoveDuration;
    std::shared_ptr<class Bmp> FireImg{};
    void FireCast();

private:
    void DirCheckAnimFileChange();

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

