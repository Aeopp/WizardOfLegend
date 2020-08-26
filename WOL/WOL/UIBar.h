#pragma once
#include "UI.h"

class UIBar :
    public UI
{
public:
    float ratio{};
    float max{ 500.f };
    float current{};
    float target{};
    float goal_time{ 1.f };
public:
    std::weak_ptr <class player_info>wp_info;

    Event update(float dt)override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    uint32_t get_layer_id()const& override;
};

