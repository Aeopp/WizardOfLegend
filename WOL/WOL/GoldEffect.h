#pragma once
#include "Effect.h"

class GoldEffect :
    public Effect
{
public:
    // �÷��̾ �ݵ�� owner �� �ʱ�ȭ ���ּ���.

    float initLife{ 2.f };
    float distance{ 30.f };
    float followSpeed{ 200.f };
    int Gold{};

    void initialize()override;
    Event update(float dt)override;
};

