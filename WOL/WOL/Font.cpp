#include "pch.h"
#include "Font.h"

Font::Font(HDC hdc, std::wstring text, int fontsize, COLORREF _color,int x,int y)
{
	SetBkMode(hdc, 1);
	SetTextColor(hdc, _color);
	hFont = CreateFont(fontsize, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH || FF_ROMAN, TEXT("»ﬁ∏’µ’±Ÿ«ÏµÂ∂Û¿Œ"));
	oldFont = (HFONT)SelectObject(hdc, hFont);
	TextOut(hdc, x, y, text.c_str(), text.size());
};

Font::~Font() noexcept
{
	DeleteObject(hFont);
}
