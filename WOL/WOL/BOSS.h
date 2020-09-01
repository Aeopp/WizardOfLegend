#pragma once
#include "Monster.h"



class BOSS :
    public Monster
{
public:
    void initialize()override;
    Event update(float dt)override;
    void Hit(std::weak_ptr<object> _target)override;
private:
    enum class AnimTable
    {
        idle,
        walk,
        attack,
        hit,
        dead,
    };
    enum class AnimAttackTable
    {
        left,
        up,
        right,
        down,
    };
    enum class EAnimDir
    {
        right,
        left,
        attack,
        max,
    };

    std::vector<int> BOSSDefaultAnimInfo{ 1,9,2,8,2,1 };
    std::vector<int> BOSSAttackAnimInfo{6,6,6,6};
    std::array<std::weak_ptr<class Bmp>, (int)EAnimDir::max> AnimDirFileTable;
};

