#include "pch.h"
#include "Debuger.h"

Debuger::Debuger(HDC hdc,std::function<void()> Work)
{
	if (bDebug)
	{
		bDebugInit = true;

		Brush = CreateSolidBrush(RGB(50, 50, 50));
		oBrush = (HBRUSH)SelectObject(hdc, Brush);
		Pen = (HPEN)CreateSolidBrush(RGB(255, 127, 182));
		oPen = (HPEN)SelectObject(hdc, Pen);

		Work();
	}
}

Debuger::~Debuger() noexcept
{
	if (bDebugInit)
	{
		SelectObject(hdc, oBrush);
		DeleteObject(Brush);
		DeleteObject(Pen);
	}
}

Debuger::Debuger()
{
	if (bDebug)
	{
		bDebugInit = true;

		Brush = CreateSolidBrush(RGB(150,20, 200));
		oBrush = (HBRUSH)SelectObject(hdc, Brush);
		Pen = (HPEN)CreateSolidBrush(RGB(121,121, 0));
		oPen = (HPEN)SelectObject(hdc, Pen);
	}
}

Debuger::Debuger(HDC hdc,  COLORREF BrushColor, COLORREF PenColor)
{
	if (bDebug)
	{
		bDebugInit = true;

		Brush = CreateSolidBrush(BrushColor);
		oBrush = (HBRUSH)SelectObject(hdc, Brush);
		Pen = (HPEN)CreateSolidBrush(PenColor);
		oPen = (HPEN)SelectObject(hdc, Pen);
	}
}
