#pragma once
#include <wtypes.h>

class Debuger
{
public:
	// ������ �ؾ��ϴ� �۾��� ����.
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

