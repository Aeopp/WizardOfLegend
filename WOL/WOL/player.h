#pragma once
#include "actor.h"
#include "collision_component.h"
#include "ESkillBarIcon.h"

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

    std::array<std::weak_ptr<class Bmp>,(int)EAnimDir::max> AnimDirFileTable;

    void temp(float temp);
    void MakeShield();
    void ICE_CRYSTAL();
    void Camera_Shake(float force, vec dir, float duration);
    void player_check(float dt);

    void make_gold_UI();
    void make_skill_bar();
    void make_player_bar();
    void make_skillbar_icon(ESkill _eSkill);
    void Dash();
 
};

