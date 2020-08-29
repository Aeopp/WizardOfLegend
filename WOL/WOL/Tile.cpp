#include "pch.h"
#include "Tile.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include <fstream>
#include <ostream>
#include "Scene_Edit.h"

void Tile::render(HDC hdc, vec camera_pos, vec size_factor)
{
	auto BMP = Bmp_mgr::instance().Find_Image_SP(Bmp_mgr::ImageSelectMap[Imagekey].second);
	if (!BMP)return;

	vec l = _location - camera_pos;
	vec s = _size;
	vec pl = _paint_location;
	vec ps = _paint_size;
	COLORREF CK = _ColorKey;

	GdiTransparentBlt(hdc, l.x, l.y, s.x, s.y, BMP->Get_MemDC(), pl.x, pl.y, ps.x, ps.y, CK);
};

std::ostream& operator<<(std::ostream& os, const Tile& _Tile)
{
	// comma
	char c{ ',' };
	// boundary
	char bd{ '|' };

	auto& T = _Tile;

	os << bd << T._location.x << c << T._location.y << c << T._size.x << c << T._size.y << c <<
		T._paint_location.x << c << T._paint_location.y << c << T._paint_size.x << c << T._paint_size.y << c
		<< (uint32_t)T.Imagekey << c << T._ColorKey << c << T.RowIndex << c << T.ColIndex << c << T.bRender << bd;

	return os;
}

std::istream& operator>>(std::istream& is, Tile& _Tile)
{
	char temp{};

	auto& T = _Tile;

	is >> temp;
	is >> T._location.x;
	is >> temp;
	is >> T._location.y;
	is >> temp;
	is >> T._size.x;
	is >> temp;
	is >> T._size.y;
	is >> temp;
	is >> T._paint_location.x;
	is >> temp;

	is >> T._paint_location.y;
	is >> temp;

	is >> T._paint_size.x;
	is >> temp;

	is >> T._paint_size.y;
	is >> temp;

	uint32_t key;
	is >> key;
	T.Imagekey = (ETileSelect)key;

	is >> temp;

	is >> T._ColorKey;
	is >> temp;

	is >> T.RowIndex;
	is >> temp;

	is >> T.ColIndex;
	is >> temp;

	is >> T.bRender;

	is >> temp;

	return is;
}
