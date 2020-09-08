#pragma once
#include "object.h"
#include "Shadow.h"

class NPC :
    public object
{
public:
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    void initialize()override;
    void SetUp(vec Location);
    std::shared_ptr<class Bmp> sp_TableImg;
    std::shared_ptr<class Bmp> sp_NpcImg;
    Shadow _Shadow;
};

