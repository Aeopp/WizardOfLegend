#pragma once
#include "UIInfo.h"
#include "player_info.h"
#include "ESkillBarIcon.h"

class UISkillIBarIcon :
    public UIInfo
{
public:
    std::weak_ptr<class Bmp> SkillBarBmpUpdate();
    void late_initialize(vec pos, std::wstring FileName);
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    Event update(float dt) override; 

    std::weak_ptr<class player_info> wp_PlayerInfo;
    void CoolTimeRefUpdate();

    static inline float Tick = 0;
    uint8_t CurrentSlotIdx;
   const float* Max{};
    float* Current{};
};

