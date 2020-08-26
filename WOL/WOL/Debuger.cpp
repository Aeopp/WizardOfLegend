#include "pch.h"
#include "Debuger.h"

Debuger::Debuger(HDC hdc,std::function<void()> Work)
{
	if (bDebug)
	{
		bDebugInit = true;

		Brush = CreateSolidBrush(RGB(50, 50, 50));
		oBrush = (HBRUSH)SelectObject(hdc, Brush);
		Pen = (HPEN)CreateSolidBrush(RGB(255, 0, 0));
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
