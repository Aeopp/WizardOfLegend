#pragma once
#include "actor.h"
#include "collision_component.h"
#include "ESkillBarIcon.h"
#include "Freezing_Interface.h"
#include "Burning_Interface.h"

enum class PlayerState : uint8_t
{
    Attack,
    Move,
    Idle,
    Dash,
    Dead,
};
class Player : public actor , public  Freezing_Interface , public Burning_Interface
{
public:
    std::weak_ptr<collision_component> _collision_component;
    std::weak_ptr<class Camera> _Camera;
    std::weak_ptr<class EffectPlayerAttack> NormalAttack{};
    std::weak_ptr<class UIInventory> wp_Inventory{};

    bool bInvenControl = false;

    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    void initialize()override;
    Event update(float dt)override;

    void Hit(std::weak_ptr<object> _target)override;

    PlayerState CurrentState{ PlayerState::Idle };

    float CurrentInvincibletime;
    float DefaultInvincibletime;
    float IceCrystalTick = 0;
    bool bInputDiagonal{ false };
    bool bUltiOn{ false };
    std::shared_ptr<class player_info> _player_info;
    void Camera_Shake(float force, vec dir, float duration);
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
    void InputDirSpriteChange(vec Dir);

    void BindingSkillCheckCast(int SlotIdx);
    void player_check(float dt);

    void MakeShield();
    void ICE_BLAST(int Num);
    void SkillIceCrystal(uint32_t Num);
    void SkillUlti();
    void SkillFireDragon();

    void SkillBoomerang();
    void MultiBoomerang(int Num);
    void SkillRotBoomerang();
    void MultiRotBoomerang(int Num);
    void SkillScrewBoomerang();
    void MultiScrewBoomerang(int Num);

    void GetSkill();

    void EnterBossStage(std::shared_ptr<class object> IsPortal);

    void SkillInCastSlowTime(float Duration,float SlowTimeScale);
    void CheckDirInput();
    void make_gold_UI();
    void make_skill_bar();
    void make_player_bar();
    void make_skillbar_icon(ESkill _eSkill);
    void Dash(float speed);
    void Attack();
    void Player_Move(float dt);
    void Die();
    void MissMsg();
};

