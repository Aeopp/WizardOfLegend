#pragma once
#include "object.h"
#include "math.h"
#include "Shadow.h"

class actor :
    public object
{
public:
    void initialize()override;

    float ScaleX;
    float ScaleY;
    int PaintSizeX;
    int PaintSizeY;
    std::shared_ptr<class render_component> _render_component{};
    Shadow _Shadow;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    float _speed{300.f};
};

