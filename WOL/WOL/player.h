#pragma once
#include "object.h"
#include "collision_component.h"

class player :
    public object
{
public:
    collision_component _collision_component;

    void initialize()override;
};

