#include "pch.h"
#include "Tile_mgr.h"
#include "game.h"
#include "object_mgr.h"
#include "helper.h"
#include "math.h"
#include <fstream>


Tile_mgr::~Tile_mgr() noexcept
{
}

void Tile_mgr::initialize()
{
	// 여기서 타일 개수만큼 메모리를 미리 예약해주세요.
	// _Tile_list.reserve(???);
	_Tile_list.resize(game::TileNumX * game::TileNumY);
}

void Tile_mgr::update(float dt)
{

}

void Tile_mgr::late_update()
{
}

void Tile_mgr::render(HDC hdc, std::pair<float,float> size_factor)
{
	vec cp = object_mgr::instance().camera_pos;

	for (auto& Tile : _Tile_list)
	{
		if (!Tile)continue;

		Tile->render(hdc, cp, vec{ size_factor.first,size_factor.second});
	}
}


void Tile_mgr::release()
{
	if (!_Tile_list.empty())
		_Tile_list.clear();
};


std::pair<int, int> Tile_mgr::CalcTileImageSrcIndex(vec mouse_window_pos, vec Image_window_pos, vec Image_start_pos, int TileImageX, int TileImageY)
{
	//마우스 좌표를 현재 창에 띄운 이미지 기준으로 바꿉니다.
	mouse_window_pos -= Image_window_pos;
	// 마우스 좌표를 창에 띄운 이미지 기준에서 인덱스로 변환할려는 이미지의 시작 부분 기준으로 바꿉니다.
	mouse_window_pos -= Image_start_pos;

	helper::TEXTOUT(CurHdc, 150, 250, L" Image Index X : ", mouse_window_pos.x / TileImageX, L" Image Index Y : ", mouse_window_pos.y / TileImageY);

	return { mouse_window_pos.x / TileImageX, mouse_window_pos.y / TileImageY };
}

std::pair<int,int> Tile_mgr::CalcTileWorldIndex(vec mouse_world_pos, int TileWorldX, int TileWorldY)
{
	helper::TEXTOUT(CurHdc, 150, 350, L" World Index X : ", (mouse_world_pos.x / TileWorldX) * TileWorldX, L" World Index Y : ", (mouse_world_pos.y / TileWorldY) * TileWorldY);

	return { (mouse_world_pos.x/ TileWorldX)  , (mouse_world_pos.y / TileWorldY)};
};


void Tile_mgr::Save_Tile()
{
	std::wstring wstr = DefaultTilePath + std::to_wstring(math::Rand<int>({ 1,10000 })) + L".dat";

	HANDLE hFile = CreateFile(wstr.c_str(), GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	HWND hWnd = game::instance().hWnd;

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(hWnd, L"Save failure !!", L"Save failure !!", MB_OK);
		return;
	}

	DWORD dwByte = 0;

	for (auto& pTile : _Tile_list)
	{
		if (!pTile->_Info.bRender)continue;
		WriteFile(hFile, &pTile->_Info, sizeof(TileInfo), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(hWnd, L"Save Success !!", L"Save Success !!", MB_OK);

	/*std::wstring wstr = DefaultTilePath + L"Tile.dat";

	std::wofstream ofs(wstr,std::ios::binary);

	for (auto& _Tile : _Tile_list)
	{
		if (!_Tile)continue;
		if (_Tile->_Info.Imagekey.empty())continue;
		ofs.write((wchar_t*)(&_Tile->_Info), sizeof(TileInfo));
	};*/
}


void Tile_mgr::Load_Tile()
{
	HANDLE hFile = CreateFile((DefaultTilePath + L"Tile.dat").c_str(), GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	HWND hWnd = game::instance().hWnd;

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(hWnd, L"Load Failure !!", L"Load Failure ", MB_OK);
		return;
	}
	release();
	initialize();

	DWORD	dwByte{ 0 };
	TileInfo	Temp{};
	while (true)
	{
		ReadFile(hFile, &Temp, sizeof(TileInfo), &dwByte, NULL);

		if (0 == dwByte)
			break;

		std::shared_ptr<Tile> _Tile = std::make_shared<Tile>();
		if (!_Tile)return;
		_Tile->_Info = Temp;

		int Index = _Tile->_Info.RowIndex * game::TileNumX + _Tile->_Info.ColIndex;
		if (game::TileNumX * game::TileNumY <= Index)
			return;
		if (0 > Index)
			return;

		
		_Tile_list[Index] = _Tile;
	}

	CloseHandle(hFile);
	MessageBox(hWnd, L"Load Success !!", L"Load Success !!", MB_OK);

	//std::wstring wstr = DefaultTilePath + L"Tile.dat";

	//std::wifstream wif(wstr);

	//TileInfo _Info;
	//while (wif)
	//{	
	//	wif.read((wchar_t*)(&_Info), sizeof(TileInfo));
	//	int Index = _Info.RowIndex* game::TileNumX + _Info.ColIndex;
	//	if (game::TileNumX * game::TileNumY <= Index)
	//		continue;
	//	if (0 > Index)
	//		continue;

	//	if (!_Tile_list[Index]) return; 

	//	_Tile_list[Index]->_Info = _Info;
	//}
}
void Tile_mgr::Erase_Tile(std::pair<int, int> WorldIndex)
{
	int Index = WorldIndex.second * game::TileNumX + WorldIndex.first;

	if (game::TileNumX * game::TileNumY <= Index)
		return;
	if (0 > Index)
		return;

	auto _Tile = _Tile_list[Index];

	if (!_Tile)return;

	_Tile->_Info = TileInfo{};
};

void Tile_mgr::Insert_Tile(ETileSelect ImageKey, std::pair<int, int> WorldSize, std::pair<int, int> ImgSize, std::pair<int, int> Paint_Loc,COLORREF ColorKey,
	std::pair<int, int> WorldIndex)
{
	auto  v = WorldIndex;
	if ( game::TileNumX * game::TileNumY <= v.second * game::TileNumX + v.first)
		return;
	if (0>v.second * game::TileNumX + v.first)
		return;

	auto sp_Tile = std::make_shared<Tile>();
	if (!sp_Tile)return;
	sp_Tile->_Info.ColIndex = WorldIndex.first;
	sp_Tile->_Info.RowIndex = WorldIndex.second;
	sp_Tile->_Info.Imagekey = ImageKey;
	sp_Tile->_Info._ColorKey = ColorKey;
	sp_Tile->_Info._location = { WorldIndex.first * game::TileWorldX, WorldIndex.second * game::TileWorldY };
	sp_Tile->_Info._paint_location = vec{ Paint_Loc.first,Paint_Loc.second };
	sp_Tile->_Info._paint_size = vec{ ImgSize.first,ImgSize.second };
	sp_Tile->_Info._size = vec{ game::TileWorldX,game::TileWorldY };
	sp_Tile->_Info.bRender = true;

	// y인덱스 곱하기 x 사이즈  + x인덱스

	_Tile_list[v.second * game::TileNumX + v.first] = sp_Tile;
}
