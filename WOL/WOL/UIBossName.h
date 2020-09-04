#pragma once
#include "UI.h"
class UIBossName :
    public UI
{
public:
    void initialize()override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    Event update(float dt)override;

    void late_initialize(const std::wstring& UIFileName, const std::wstring& UIKey);

    std::weak_ptr<class      player_info > _player_info;
};

