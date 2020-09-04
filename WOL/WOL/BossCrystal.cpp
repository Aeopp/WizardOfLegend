#include "pch.h"
#include "BossCrystal.h"
#include "render_component.h"
#include "collision_component.h"
#include "collision_mgr.h"
#include "Bmp_mgr.h"
#include "sound_mgr.h"

void BossCrystal::initialize()
{
    actor::initialize();

    _collision_component = collision_mgr::instance().insert
    (_ptr, collision_tag::EMonsterAttack, ECircle);

    auto sp_collision = _collision_component.lock();

    if (!sp_collision)return;
    sp_collision->_size = { 25.f,25.0f };

    PaintSizeX = 200;
    PaintSizeY = 200;
    float Scale = 0.8f;
    // 자기자신의 회전속도임

    _render_component = std::make_shared<render_component>();
    _render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(L"ICE_CRYSTAL");
    _render_component->Default_Src_Paint_Size = vec{ PaintSizeX,PaintSizeY };
    _render_component->Dest_Paint_Size = vec{ PaintSizeX * Scale,PaintSizeY * Scale };
    _render_component->_ColorKey = RGB(200, 230, 250);
    _render_component->_Img_src = RECT{ 0,0,PaintSizeX,PaintSizeY };
 

    Attack = { 20,40 };
    bAttacking = true;
    TickScale = 1.f;
    sp_collision->bSlide = false;
    sp_collision->bCollisionTargetPushFromForce = true;
    sp_collision->PushForce = 10.f;
    sp_collision->bCollision = true;
    sp_collision->bHitEffect = true;

    sound_mgr::instance().Play("ICE_KRYSTAL_START", false, 1.f);

    UniqueID = EObjUniqueID::ICECRYSTAL;
    Duration = 15.f;
    _speed = 120.f;
    RotationSpeedDegree = 120.f;
    DistancefromCenter = 50.f;

    _render_component->_Anim.SetAnimationClip(
        { 18 }, 360.f / _speed);
    ObjectTag = object::Tag::monster_attack;

   

};

void BossCrystal::late_initialize(vec RotationCenter,float RotationDuration,
    std::weak_ptr<class object> wp_AttackTarget/*Owner*/,float AttackSpeed)
{
    this->RotationCenter = RotationCenter;
    this -> RotationDuration= RotationDuration; 
    this->_owner = wp_AttackTarget;
    this->AttackSpeed = AttackSpeed;
}

void BossCrystal::Hit(std::weak_ptr<class object> wp_Target)
{
    ICE_Crystal::Hit(wp_Target);
}

Event BossCrystal::update(float dt)
{
    Event _Event =actor::update(dt);

    if (Duration < 0)return Event::Die;
    
    RotationDuration -= dt;
    Duration -= dt;

    if (RotationDuration > 0)
    {
        Tick += dt;
        float c = (cosf(Tick) * TickScale) + TickScale;

        float _RotationSpeedDegree = RotationSpeedDegree * c;
        float  _DistancefromCenter = DistancefromCenter * c;
        float Animspeed = (360.f / _speed * c);

        if (!_render_component)return Event::Die;
        _render_component->_Anim.AnimDuration = Animspeed;
      
        _transform->_dir = math::rotation_dir_to_add_angle
        (_transform->_dir, _RotationSpeedDegree * dt);

        _transform->_location = RotationCenter + _transform->_dir *
            (DistancefromCenterMin + _DistancefromCenter);
    }

    if (RotationDuration < 0)
    {
        CalcAttackDir();
    }

    if (Duration>0 && RotationDuration<0)
    {
        _transform->_dir = AttackDir;
        _transform->_location += _transform->_dir * AttackSpeed*DeltaTime;
    }

    return _Event;
}

uint32_t BossCrystal::get_layer_id() const&
{
    return layer_type::EEffect;
}

void BossCrystal::CalcAttackDir()
{
    if (!bAttack)
    {
        auto sp_AttackTarget = _owner.lock();
        if (!sp_AttackTarget)return;
        vec TargetLocation = sp_AttackTarget->_transform->_location;
        vec Dis = TargetLocation - _transform->_location;
        AttackDir = Dis.get_normalize();
        bAttack = true;
    }
}
