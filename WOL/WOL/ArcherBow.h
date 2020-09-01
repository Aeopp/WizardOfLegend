#pragma once
#include "object.h"
#include "event.h"

class ArcherBow :
    public object
{
public:
    // f = LEFT s = RIGHT
    // 각도 20도마다 스프라이트 있음
    // 360도 기준 앵글에 20 나눠서 테이블 접근
    // 활과 화살 둘다 이 로우인덱스로 접근
    // 4~14
    static inline std::pair<
        std::map<uint32_t, uint32_t>,
        std::map<uint32_t, uint32_t>> BowSpriteAngleTable
    { {
        {4,0}  ,
        {5,0}  ,
        {6,1}  ,
        {7,2}  ,
        {8,3}  ,
        {9,4}  ,
        {10,5} ,
        {11,6} ,
        {12,7} ,
        {13,8} ,
        {14,9} 
    },
        // 0 ~ 4
        // 13 ~ 18
    {
        {4,0},
        {3,1},
        {2,2},
        {1,3},
        {0,4},
        {18,4},
        {17,5},
        {16,6},
        {15,7},
        {14,8},
        {13,8}
    }};

    void initialize();
    Event update(float dt)override;
    void HitTile(RECT rt)override;
    uint32_t get_layer_id()const&override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    void Hit(std::weak_ptr<object> _target)override; 

    int CurrentBowRow{};
    int CurrentBowCol{};
    float CurrentBowAnimDelta;
    float DefaultBowAnimDelta;
    int BowPaintSizeX = 180;
    int BowPaintSizeY = 170;
    float BowScaleX;
    float BowScaleY;
   

    vec BowCorrection;
    std::shared_ptr<class Bmp> sp_Img;
    std::weak_ptr<class object> wp_Target;

    void Preparation(bool Set);
    void launch(vec init,vec dir);
    void end();
    float speed{};
    bool bPreparation = false;
};

