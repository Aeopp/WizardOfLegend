#pragma once
#include "Effect.h"

class GoldEffect :
    public Effect
{
public:
    static std::shared_ptr<GoldEffect> MakeGold(int ImgLocationX, int ImgLocationY,
        std::wstring ImgKey, layer_type layer_ID, int AnimColNum,
        int AnimRowIndex, float Duration, float AnimDuration,
        int PaintSizeX, int PaintSizeY, float ScaleX, float ScaleY,
        std::weak_ptr<class object> _Owner);

    // �÷��̾ �ݵ�� owner �� �ʱ�ȭ ���ּ���.

    float initLife{ 1.f };
    float distance{ 30.f };
    float followSpeed{ 1000.f };
    int Gold{};

    void initialize()override;
    Event update(float dt)override;
};

