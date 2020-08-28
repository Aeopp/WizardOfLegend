#pragma once
#include "Monster.h"

class WIZARD :
    public Monster
{
public:
    void initialize()override;
    Event update(float dt)override;
    void Hit(std::weak_ptr<object> _target)override;
    void Attack()override;
private:
    enum class AnimTable
    {
        idle,
        walk,
        attack,
        hit,
        dead,
    };
    enum class EAnimDir
    {
        right,
        left,
        max,
    };
    std::array<std::weak_ptr<class Bmp>, (int)EAnimDir::max> AnimDirFileTable;
};

