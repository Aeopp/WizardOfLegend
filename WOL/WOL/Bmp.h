#pragma once
#include "pch.h"

class Bmp
{
public:
	~Bmp()noexcept;
public:
	HDC Get_MemDC() { return m_hMemDC; }
	void Load_Bmp(const std::wstring& FileName);
	void Release();
private:
	HDC			m_hMemDC{};
	HBITMAP		m_hBitmap{};
	HBITMAP		m_hOldBmp{};
};