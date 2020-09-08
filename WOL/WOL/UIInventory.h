#pragma once
#include "UIInfo.h"
#include "ESkillBarIcon.h"

struct SlotInfo
{
    static inline vec Size{44,44 };
    static inline vec SelectSize = { 52,52 }; 
    vec Location; 
    std::wstring ImgKey; 
    std::wstring InfoKey;
    ESkill _Skill;
    bool bAcquire = false;
};
class UIInventory :
    public UIInfo
{
public:
    static inline std::unordered_map<uint8_t, SlotInfo> SlotInfoMap{};
    void initialize()override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    uint32_t get_layer_id()const& override;
    Event update(float dt)override;

    bool bSelect = false;
    bool bInventoryOpen = false;
    float ChangeCoolTime = 0.1f;

    uint8_t CurrentInfoIdx =  0 ;

    uint8_t CurrentSelectIdx = 2 ;
};

