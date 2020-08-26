#pragma once
class Font
{
public:
	Font(HDC hdc, std::wstring text, int fontsize = 30, COLORREF _color
		= RGB(255, 255, 255), int x = 10, int y = 10);
		~Font()noexcept;
private:

		HFONT hFont, oldFont;

};

