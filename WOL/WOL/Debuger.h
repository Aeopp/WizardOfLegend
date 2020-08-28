#pragma once
#include <wtypes.h>

class Debuger
{
public:
	// 디버깅시 해야하는 작업시 생성.
	Debuger(HDC hdc, std::function<void()> Work);
	virtual ~Debuger()noexcept;
	Debuger();
	Debuger(HDC hdc,  COLORREF BrushColor, COLORREF PenColor);
private:
	bool bDebugInit = false;
	HBRUSH Brush, oBrush;
	HDC hdc;
	HPEN Pen, oPen;
};

