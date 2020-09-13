#pragma once
#include "object.h"


// �̹��� �������� �ݵ�� �������ּ���.
// �������� �������� �ݵ�� �������ּ���.
// �ݸ��� ������Ʈ�� ����� �̹����� ����ȭ �����ּ���.
// Owner �� �ݵ�� Ʈ���ŷ� �������ּ���.
class Prison :
    public object
{
public :
    // ���� ������ ����
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

