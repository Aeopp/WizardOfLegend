#pragma once
#include "Effect.h"

class GoldEffect :
    public Effect
{
public:
    // 플레이어가 반드시 owner 를 초기화 해주세요.

    float initLife{ 2.f };
    float distance{ 30.f };
    float followSpeed{ 200.f };
    int Gold{};

    void initialize()override;
    Event update(float dt)override;
};

