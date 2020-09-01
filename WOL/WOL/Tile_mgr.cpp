#include "pch.h"
#include "Tile_mgr.h"
#include "game.h"
#include "object_mgr.h"
#include "helper.h"
#include "math.h"
#include <istream>
#include <fstream>
#include <ostream>


Tile_mgr::~Tile_mgr() noexcept
{
}

void Tile_mgr::initialize()
{
	// 여기서 타일 개수만큼 메모리를 미리 예약해주세요.
}

void Tile_mgr::update(float dt)
{

}

void Tile_mgr::late_update()
{

}

void Tile_mgr::render(HDC hdc, vec camera_pos , std::pair<float,float> size_factor)
{
	/*vec cp = object_mgr::instance().camera_pos;*/

	for (auto& _Tile : _Tile_list)
	{
		if (!_Tile.bRender)continue;
		RECT CameraRange = game::client_rect;
		CameraRange.left -= _Tile._size.x;
		CameraRange.top -= _Tile._size.y;
		
		if (!math::RectInPoint(CameraRange, _Tile._location - camera_pos))continue;

		if (_Tile.bDeco)
		{
			DecoVec.push_back(_Tile);
			continue; 
		}

		_Tile.render(hdc, camera_pos, vec{ size_factor.first,size_factor.second });
	};
}

void Tile_mgr::DecoRender(HDC hdc,vec camera_pos)
{
	for (auto& _Tile : DecoVec)
	{
		_Tile.get().render(hdc, camera_pos, vec{});
	}
	DecoVec.clear();
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

//	helper::TEXTOUT(game::CurrentHdc, 150, 250, L" Image Index X : ", mouse_window_pos.x / TileImageX, L" Image Index Y : ", mouse_window_pos.y / TileImageY);

	return { mouse_window_pos.x / TileImageX, mouse_window_pos.y / TileImageY };
}

std::pair<int, int> Tile_mgr::CalcTileWorldIndex(vec mouse_world_pos, int TileWorldX, int TileWorldY)
{
	if(bDebug)
	helper::TEXTOUT(game::CurrentHdc, 150, 350, L" World Index X : ", (mouse_world_pos.x / TileWorldX) , L" World Index Y : ", (mouse_world_pos.y / TileWorldY) );

	return { (mouse_world_pos.x / TileWorldX)  , (mouse_world_pos.y / TileWorldY) };
};

bool Tile_mgr::IsContain(std::pair<int, int> WorldIndex) const
{
	auto iter = std::find_if(std::begin(_Tile_list), std::end(_Tile_list), [WorldIndex](auto& _Tile)
		{return WorldIndex.first == _Tile.ColIndex && WorldIndex.second == _Tile.RowIndex; });

	return iter != std::end(_Tile_list);
};

void Tile_mgr::Save_Tile(std::wstring filename)
{
	size_t Tile_Num  = _Tile_list.size();
	auto FileName = std::to_wstring(math::Rand<uint32_t>({ 2020,7070 }));

	std::ofstream ofs(Tile_mgr::DefaultTilePath + FileName);

	if (!ofs.is_open())
	{
		MessageBox(game::hWnd, L"타일 데이터를 저장하지 못했습니다.", L"Tile Load Fail !!", MB_OK);
		//throw std::exception("Tile Data Load Fail !!");
	}
	
	ofs << Tile_Num;

	for (auto& _Tile : _Tile_list)
	{
		ofs << _Tile;
	}

	std::wstringstream
		wss;
	wss << L"타일  데이터를 저장했습니다. \n " << L" 파일 이름 : " << FileName;
	MessageBox(game::hWnd, wss.str().c_str(), L"Tile Data Load",  MB_OK);
}


void Tile_mgr::Load_Tile(std::wstring filename)
{
	if (!_Tile_list.empty())
		_Tile_list.clear();

	std::ifstream ifs(Tile_mgr::DefaultTilePath + filename);

	if (!ifs.is_open())
	{
		MessageBox(game::hWnd, L"타일 데이터를 읽어오지 못했습니다.", L"Tile Load Fail !!", MB_OK);
		throw std::exception("Tile Data Load Fail !!");
	}

	size_t Tile_Num;
	ifs >> Tile_Num;

	_Tile_list.reserve(Tile_Num);

	for (int i = 0; i < Tile_Num; ++i)
	{
		Tile _Temp;
		ifs>> _Temp;
		_Tile_list.push_back(std::move(_Temp));
	}

	std::wstringstream
		wss;
	wss <<  L"타일 데이터를 로딩했습니다. \n " << DefaultTilePath << filename;
	MessageBox(game::hWnd, wss.str().c_str(), L"Tile Data Load",  MB_OK);
}
void Tile_mgr::Erase_Tile(std::pair<int, int> WorldIndex)
{
	std::erase_if(_Tile_list, [WorldIndex](auto& _Tile)
		{return WorldIndex.first == _Tile.ColIndex && WorldIndex.second == _Tile.RowIndex; });
};



void Tile_mgr::Insert_Tile(ETileSelect ImageKey, std::pair<int, int> WorldSize, std::pair<int, int> ImgSize, std::pair<int, int> Paint_Loc, COLORREF ColorKey,
	std::pair<int, int> WorldIndex,bool bDeco)
{
	if (bDeco)
	{
		if (_Deco_Tile_Indexs.contains(WorldIndex))return;

		_Deco_Tile_Indexs.insert(WorldIndex);
	}

	auto  v = WorldIndex;
	if (game::TileNumX * game::TileNumY <= v.second * game::TileNumX + v.first)
		return;
	if (0 > v.second * game::TileNumX + v.first)
		return;

	Tile _Tile;
	_Tile.ColIndex = WorldIndex.first;
	_Tile.RowIndex = WorldIndex.second;
	_Tile.Imagekey = ImageKey;
	_Tile._ColorKey = ColorKey;
	_Tile._location = { WorldIndex.first * game::TileWorldX, WorldIndex.second * game::TileWorldY };
	_Tile._paint_location = vec{ Paint_Loc.first,Paint_Loc.second };
	_Tile._paint_size = vec{ ImgSize.first,ImgSize.second };
	_Tile._size = vec{ WorldSize.first,WorldSize.second };
	_Tile.bRender = true;
	_Tile.bDeco = bDeco;
	// y인덱스 곱하기 x 사이즈  + x인덱스
	_Tile_list.push_back(_Tile);
};
