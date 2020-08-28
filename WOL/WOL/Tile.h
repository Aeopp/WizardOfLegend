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
    // 월드에서의 타일의 (그릴때에도 사용)위치입니다. 
    // 타일은 예외적으로 좌상단을 기준으로 잡습니다.
    // 피킹시에 월드상에서의 위치를 계산해서 넘겨주세요.
    vec _location;
    // 월드 상에서의 타일의 사이즈입니다.
    vec _size;
    // 이미지 상에서의 타일의 위치입니다. 역시 좌상단을 기준으로 잡습니다.
    // 마찬가지로 피킹시에 계산해서 넘겨주세요.
    vec _paint_location;
    // 이미지상에서의 타일의 사이즈입니다.
    vec _paint_size;

    // 이미지의 키입니다.
    ETileSelect Imagekey;


    COLORREF _ColorKey{ COLOR::MEGENTA() };
    // 이미지 에서의 인덱스입니다.
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

