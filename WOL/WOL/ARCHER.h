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
};

