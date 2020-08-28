#pragma once
#include "transform.h"
#include "Color.h"

enum class ETileSelect : uint8_t
{
    Info_1,
    Info_2,
    Info_3,
    Info_4,
};

struct TileInfo
{
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
};

std::wostream& operator<<(std::wostream& os, const TileInfo& _TileInfo);

std::wfstream& operator>>(std::wfstream& fs, TileInfo& _TileInfo);


class Tile
{
public:
    void render(HDC hdc, vec camera_pos, vec size_factor);
    TileInfo _Info;
};

