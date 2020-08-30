#pragma once
#include "actor.h"
#include "collision_component.h"
#include "ESkillBarIcon.h"

enum class PlayerState : uint8_t
{
    Attack,
    Move,
    Idle,
    Dash,
    Dead,
};
class Player :
    public actor
{
public:
    std::weak_ptr<collision_component> _collision_component;
    std::weak_ptr<class Camera> _Camera;

    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    void initialize()override;
    Event update(float dt)override;

    void Hit(std::weak_ptr<object> _target)override;

    PlayerState CurrentState{ PlayerState::Idle };

    std::shared_ptr<class player_info> _player_info;
private:
    enum class AnimTable
    {
        idle,
        walk,
        dash,
        attack1,
        attack2,
        hit,
        dead,
        max,
    };

    enum class EAnimDir
    {
        front,
        back,
        right,
        left,
        max,
    };

    void StateCheck();

    std::array<std::weak_ptr<class Bmp>,(int)EAnimDir::max> AnimDirFileTable;

    void temp(float temp);
    void MakeShield();
    void ICE_BLAST(int Num);
    void Camera_Shake(float force, vec dir, float duration);
    void player_check(float dt);

    void SkillIceCrystal(uint32_t Num);
    void SkillFireDragon();

    void CheckDirInput();
    void make_gold_UI();
    void make_skill_bar();
    void make_player_bar();
    void make_skillbar_icon(ESkill _eSkill);
    void Dash(float speed);
    void Attack();
    void Player_Move(float dt);
};

