#pragma once
#include "object.h"
#include "collision_component.h"

class Player :
    public object
{
public:
    std::shared_ptr<collision_component> _collision_component;

    void initialize()override;
    void update()override;
    void render(HDC hdc)override;
};

