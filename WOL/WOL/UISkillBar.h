#pragma once
#include "UI.h"
class UISkillBar :
    public UI
{
public:
    void initialize()override;
    void render(HDC hdc, vec camera_pos,vec size_factor)override;

    uint32_t get_layer_id()const&override;
};

