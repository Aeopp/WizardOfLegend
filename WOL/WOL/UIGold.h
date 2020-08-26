#pragma once
#include "UI.h"
#include <functional>

class UIGold :
    public UI
{
public:
    void initialize()override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    uint32_t get_layer_id()const& override;

    std::weak_ptr<class player_info> _player_info;
};

