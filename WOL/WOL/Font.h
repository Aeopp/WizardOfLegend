#pragma once
class Font
{
public:
	Font(HDC hdc, std::wstring text, int fontsize = 30, COLORREF _color
		= RGB(255, 255, 255), int x = 10, int y = 10);
		~Font()noexcept;

		template<typename ... Strs>
		Font(HDC hdc,   COLORREF _color, int x , int y , int fontsize ,
		Strs&&... text);
private:
		HFONT hFont, oldFont;
};

template<typename ...Strs>
inline Font::Font(HDC hdc, COLORREF _color, int x, int y, int fontsize, Strs&& ...text)
{
	SetBkMode(hdc, 1);
	SetTextColor(hdc, _color);
	hFont = CreateFont(fontsize, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH || FF_ROMAN, TEXT("»ﬁ∏’µ’±Ÿ«ÏµÂ∂Û¿Œ"));
	oldFont = (HFONT)SelectObject(hdc, hFont);

	(TextOut(hdc, x, y, text.c_str(), text.size()), ...);
}
