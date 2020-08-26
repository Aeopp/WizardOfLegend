#pragma once
#include "UIBar.h"
class UIMPBar :
    public UIBar
{
public:

    void initialize()override;
    Event update(float dt)override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    uint32_t get_layer_id()const& override;
};

