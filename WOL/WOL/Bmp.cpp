#include "pch.h"
#include "Bmp.h"
#include "game.h"

Bmp::~Bmp() noexcept
{
	Release();
};

void Bmp::Load_Bmp(const std::wstring& FileName)
{
	HWND _hWnd = game::instance().hWnd;

	HDC CurrentHdc = GetDC(_hWnd);

	m_hMemDC = CreateCompatibleDC(CurrentHdc);
	ReleaseDC(_hWnd, CurrentHdc);

	m_hBitmap = (HBITMAP)LoadImage(NULL, FileName.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
};

void Bmp::Release()
{
	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
