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

    float StalkerDuration;

    static inline const std::wstring SummonCardImgKey = L"SUMMON_CARD_SWORDMAN";

    std::weak_ptr<class SwordManAttack> NormalAttack{};

    /*static const inline std::unordered_map<EMonsterState, uint32_t> AnimRowIdxTable
    {
        { EMonsterState::Attack, 2},
        {EMonsterState::Dead,4},
        {EMonsterState::Hit,3},
        {EMonsterState::Idle,0},
        {EMonsterState::Walk,1}
    };*/
private:
    enum class EAnimState : uint8_t
    {
        Idle,
        Walk,
        Attack,
        Hit,
        Dead,
    };
};

