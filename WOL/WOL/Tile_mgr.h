#pragma once
#include "singleton_interface.h"
#include <windef.h>
#include "math.h"
#include "Tile.h"

class Tile_mgr :
    public singleton_interface< Tile_mgr>
{
public :
    ~Tile_mgr()noexcept;
    void initialize();
    void update(float dt);
    void late_update();
    void render(HDC hdc, std::pair<float, float> size_factor);
    void release();
public:
    void Save_Tile();
    void Load_Tile();

    void Erase_Tile(std::pair<int, int> WorldIndex);

    void Insert_Tile(ETileSelect ImageKey, std::pair<int, int> WorldSize, std::pair<int, int> ImgSize, std::pair<int, int> Paint_Loc, COLORREF ColorKey,
        std::pair<int, int> WorldIndex);
    // ���콺 ��ǥ�� �̹����� �� �� �ε����� ��ȯ���ݴϴ�.
// ���� ������ â�� ��� �̹����� ��ġ�� ������� �������� �н����ּ���.
// Ÿ���� ������� �̹����� Ÿ�� �ϳ��� ���̷� �н����ּ���.
    std::pair<int, int>  CalcTileImageSrcIndex(vec mouse_window_pos, vec Image_window_pos, vec Image_start_pos, int TileImageX, int TileImageY);
    // ���콺 ��ǥ�� ������ Ÿ�� ��ǥ�� ��� �Ѱ��� �ε����� ��Ī�����ݴϴ�.
    // ���콺�� ���� ��ǥ�� Ÿ���� ������� ����� �н����ּ���.
    std::pair<int, int>  CalcTileWorldIndex(vec mouse_world_pos, int TileWorldX, int TileWorldY);
private:
    std::vector<std::shared_ptr<class Tile>> _Tile_list;

    static inline const std::wstring DefaultTilePath = L"..\\..\\Resources\\MapInfo\\";
};

 