#pragma once
#include "RotationBoomerang.h"
class ScrewBoomerang :
    public RotationBoomerang
{
public:
    Event update(float dt)override;
    float AngleSpeed = 45.f;
    vec CurrentRotationDir{};
    float RotationCenterDistance = 100.f;
    vec RotationCenter{};
    void late_initialize(vec RotationInitCenter,
        vec RotationInitDir);
    void HitTile(RECT TileRt) override;
  
};

