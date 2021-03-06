#pragma once
#include "UI.h"
class UIInteraction :
    public UI
{
public:
    float SoundTick = 0.2f;

    virtual void OnMouse(const vec hit_pos)abstract;
    virtual void Click(const vec hit_pos)abstract;
    Event update(float dt)override;
};

