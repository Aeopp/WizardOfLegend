#pragma once
#include "object.h"
#include "ESceneID.h"

class Teleport :
    public object
{
public:
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    void initialize()override;
    void SetUp(vec Location, bool bEnding=false, ESceneID _TargetScene =ESceneID::ENone);
    uint32_t get_layer_id()const&override;
    std::weak_ptr<class collision_component> _collision_component;
    std::shared_ptr<class Bmp> sp_Bmp;
    bool bChangeScene = false;
    ESceneID _TargetScene{};
};

