#pragma once
#include "Monster.h"

class ARCHER :
    public Monster
{
public:
public:
    void initialize()override;
    Event update(float dt)override;
    void Hit(std::weak_ptr<object> _target)override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;

    static inline const std::wstring SummonCardImgKey = L"SUMMON_CARD_ARCHER";
    float CurrentFireCoolTime;
    float AttackPreparation = 1.f;
    vec EscapeVec{};
    float EscapeDuration;
    float EscapeRamainTick;
    float StalkerPosReTargetDuration;
    std::shared_ptr<class ArcherBow> NormalAttack{};
    std::shared_ptr<class Bmp> FireImg{};

    bool bArrowLineRender = false; 
    vec TargetPositionRenderPos{};
private:
    void SetArrowLineRender(bool SetFlag);
    static inline const float AttackPreparationTimeMax = 1.8f;
    float AttackPreparationTime = 0.0f;
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

