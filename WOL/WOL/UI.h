#pragma once
#include "object.h"

class UI : public object
{
public :
    void initialize()override;
    void render(HDC hdc,vec camera_pos,vec size_factor)override;
    uint32_t get_layer_id()const& override;

    std::shared_ptr<class render_component> _render_component;
};

