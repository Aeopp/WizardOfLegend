#pragma once
#include "UIInfo.h"
#include "player_info.h"
class UISkillIBarIcon :
    public UIInfo
{
public:
    void late_initialize(vec pos, std::wstring FileName);
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    Event update(float dt) override; 

    static inline float Tick = 0;

    float* Max{};
    float* Current{};
};

