#include "pch.h"
#include "Money.h"
#include "Bmp_mgr.h"

Event Money::update(float dt)
{
    // 플레이어와 거리가 가까워 지면 Duration 음수로 으로 만들어버리고 다이 트루로 만들어 버리기
    return Effect::update(dt);
};
void Money::initialize()
{
    int HowMuch = math::Rand_Normal<int>({ 1,30 });

    // 금은동
    if (HowMuch <= 10)
    {
        
    }
    else if (HowMuch <= 20)
    {

    }
    else
    {

    }


    Effect::initialize();
};

uint32_t Money::get_layer_id() const&
{
    return layer_ID;
};

