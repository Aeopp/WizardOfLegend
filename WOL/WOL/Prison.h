#pragma once
#include "object.h"


// 이미지 스케일을 반드시 설정해주세요.
// 수평인지 수직인지 반드시 설정해주세요.
// 콜리전 컴포넌트의 사이즈를 이미지와 동기화 시켜주세요.
// Owner 를 반드시 트리거로 설정해주세요.
class Prison :
    public object
{
public :
    // 수평 수직만 존재
    enum class EType : uint8_t
    {
        Hor,
        // 292 283
        Ver,
        // 23 427
    };
    Event update(float dt)override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    void initialize()override;
    void release()override;

    void SetUp(float ScaleX,float ScaleY,Prison::EType _Type,
        std::weak_ptr<class object> _owner, std::pair<int, int> CollisionSize
    ,vec Location);
    void SetCollisionCorrection(vec Correction);
private:
    std::shared_ptr<class Bmp> sp_Img;
    std::weak_ptr<class collision_component> wp_collision;
    float ScaleX;
    float ScaleY;
    int PaintSizeX;
    int PaintSizeY;
};

