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
    void render(HDC hdc, vec camera_pos , std::pair<float, float> size_factor);
    void DecoRender(HDC hdc, vec camera_pos);
    void release();
public:
    void Save_Tile(std::wstring filename= StageFileName);
    void Load_Tile(std::wstring filename= StageFileName);

    void Erase_Tile(std::pair<int, int> WorldIndex);
    void Insert_Tile(ETileSelect ImageKey, std::pair<int, int> WorldSize, std::pair<int, int> ImgSize, std::pair<int, int> Paint_Loc, COLORREF ColorKey,
        std::pair<int, int> WorldIndex, bool bDeco = false);
    // 마우스 좌표를 이미지의 행 열 인덱스로 변환해줍니다.
// 현재 윈도우 창에 띄운 이미지의 위치는 좌측상단 기준으로 패스해주세요.
// 타일의 사이즈는 이미지의 타일 하나의 넓이로 패스해주세요.
    std::pair<int, int>  CalcTileImageSrcIndex(vec mouse_window_pos, vec Image_window_pos, vec Image_start_pos, int TileImageX, int TileImageY);
    // 마우스 좌표를 월드의 타일 좌표중 어느 한곳의 인덱스와 매칭시켜줍니다.
    // 마우스의 월드 좌표와 타일의 월드기준 사이즈를 패스해주세요.
    std::pair<int, int>  CalcTileWorldIndex(vec mouse_world_pos, int TileWorldX, int TileWorldY);

    bool IsContain(std::pair<int, int> WorldIndex)const;
    static inline const std::wstring DefaultTilePath = L"..\\..\\Resources\\MapInfo\\";
    static inline const std::wstring BossStageFileName = L"BossTileInfo.txt";
    static inline const std::wstring StageFileName = L"StageTileInfo.txt";
private:
    std::vector<std::reference_wrapper<Tile>> DecoVec;
    std::vector<Tile> _Tile_list;
    // 데코레이션 타일들 중복삽입 방지
    std::set<std::pair<int, int>> _Deco_Tile_Indexs;
};

 