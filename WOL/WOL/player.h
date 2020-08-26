#pragma once
#include "actor.h"
#include "collision_component.h"

class Player :
    public actor
{
public:
    std::shared_ptr<collision_component> _collision_component;
    std::weak_ptr<class Camera> _Camera;

    void initialize()override;
    Event update(float dt)override;
    void render(HDC hdc,vec camera_pos)override;

    
private:
    void temp(float temp);
    void MakeShield();
    void ICE_CRYSTAL();
    void Camera_Shake(float force, vec dir, float duration);
    void player_check(float dt);
};

