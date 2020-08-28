#pragma once
#include "transform.h"
#include "Color.h"

enum class ETileSelect : uint32_t
{
    Info_1,
    Info_2,
    Info_3,
    Info_4,
};


class Tile
{
public:
    void render(HDC hdc, vec camera_pos, vec size_factor);
    // ���忡���� Ÿ���� (�׸������� ���)��ġ�Դϴ�. 
    // Ÿ���� ���������� �»���� �������� ����ϴ�.
    // ��ŷ�ÿ� ����󿡼��� ��ġ�� ����ؼ� �Ѱ��ּ���.
    vec _location;
    // ���� �󿡼��� Ÿ���� �������Դϴ�.
    vec _size;
    // �̹��� �󿡼��� Ÿ���� ��ġ�Դϴ�. ���� �»���� �������� ����ϴ�.
    // ���������� ��ŷ�ÿ� ����ؼ� �Ѱ��ּ���.
    vec _paint_location;
    // �̹����󿡼��� Ÿ���� �������Դϴ�.
    vec _paint_size;

    // �̹����� Ű�Դϴ�.
    ETileSelect Imagekey;
    COLORREF _ColorKey{ COLOR::MEGENTA() };
    // �̹��� ������ �ε����Դϴ�.
    int RowIndex;
    int ColIndex;

    bool bRender = false;

    friend std::ostream& operator<<(std::ostream& os, const Tile& _Tile);
    friend std::istream& operator>>(std::istream& fs,Tile& _Tile);
};

