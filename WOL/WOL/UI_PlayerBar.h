#pragma once
#include "UI.h"

class UI_PlayerBar :
    public UI
{
public:
    void initialize()override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    Event update(float dt)override;

    void late_initialize();

    std::weak_ptr<class      player_info > _player_info;
};

