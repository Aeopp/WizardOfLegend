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

    float CoolTime;
    float AttackPreparation = 1.f;

    std::shared_ptr<class ArcherBow> NormalAttack{};

    std::shared_ptr<class Bmp> BowImg{};
    /*static const inline std::unordered_map<EMonsterState, uint32_t> AnimRowIdxTable
    {
        { EMonsterState::Attack, 2},
        {EMonsterState::Dead,4},
        {EMonsterState::Hit,3},
        {EMonsterState::Idle,0},
        {EMonsterState::Walk,1}
    };*/
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

