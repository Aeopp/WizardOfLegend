#pragma once
#include "actor.h"
#include "collision_component.h"

class Player :
    public actor
{
public:
    std::shared_ptr<collision_component> _collision_component;

    void initialize()override;
    void update(float dt)override;
    void render(HDC hdc,vec camera_pos)override;

private:
    void player_move(float dt);
};

