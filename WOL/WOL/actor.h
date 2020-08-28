#pragma once
#include "object.h"
#include "math.h"

class actor :
    public object
{
public:
    std::shared_ptr<class render_component> _render_component{};
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    float _speed{300.f};
};

