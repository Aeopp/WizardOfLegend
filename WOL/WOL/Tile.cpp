#include "pch.h"
#include "Tile.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include <fstream>
#include <ostream>
#include "Scene_Edit.h"

void Tile::render(HDC hdc, vec camera_pos, vec size_factor)
{
	
	auto BMP = Bmp_mgr::instance().Find_Image_SP(Scene_Edit::ImageSelectMap[_Info.Imagekey].second);
	if (!BMP)return;

	vec l = _Info._location - camera_pos;
	vec s = _Info._size;
	vec pl = _Info._paint_location;
	vec ps = _Info._paint_size;
	COLORREF CK = _Info._ColorKey;

	GdiTransparentBlt(hdc, l.x, l.y, s.x, s.y, BMP->Get_MemDC(), pl.x, pl.y, ps.x, ps.y, CK);
}

std::wostream& operator<<(std::wostream& os, const TileInfo& _TileInfo)
{
	return os << _TileInfo.bRender << _TileInfo.ColIndex << (uint32_t)_TileInfo.Imagekey << _TileInfo.RowIndex
		<< _TileInfo._ColorKey << _TileInfo._location.x << _TileInfo._location.y << _TileInfo._paint_location.x << _TileInfo._paint_location.y  
		<< _TileInfo._paint_size.x << _TileInfo._paint_size.y 
		<< _TileInfo._size.x << _TileInfo._size.y;
}

std::wfstream& operator>>(std::wfstream& fs,  TileInfo& _TileInfo)
{
	fs >> _TileInfo.bRender;
	fs >> _TileInfo.ColIndex;
	uint32_t temp;
	fs >> temp;
	_TileInfo.Imagekey = (decltype(_TileInfo.Imagekey))temp;
	fs >> _TileInfo.RowIndex;
	fs >> _TileInfo._ColorKey;
	fs >> _TileInfo._location.x;
	fs >> _TileInfo._location.y;

	fs >> _TileInfo._paint_location.x;
	fs >> _TileInfo._paint_location.y;

	fs >> _TileInfo._paint_size.x;
	fs >> _TileInfo._paint_size.y;
	fs >> _TileInfo._size.x;
	fs >> _TileInfo._size.y;

	 return fs;
}
