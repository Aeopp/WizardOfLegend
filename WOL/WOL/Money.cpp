#include "pch.h"
#include "Money.h"
#include "Bmp_mgr.h"

Event Money::update(float dt)
{
    // �÷��̾�� �Ÿ��� ����� ���� Duration ������ ���� ���������� ���� Ʈ��� ����� ������
    return Effect::update(dt);
};
void Money::initialize()
{
    int HowMuch = math::Rand_Normal<int>({ 1,30 });

    // ������
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

