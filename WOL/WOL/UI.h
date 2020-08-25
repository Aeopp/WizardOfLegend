#pragma once
#include "object.h"
class UI :
    public object
{
public :
    void render(HDC hdc,vec camera_pos)override;
};

