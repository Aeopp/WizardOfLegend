#pragma once
#include <sstream>
class helper
{
public:
	template<typename ...Ty>
	static inline void TEXTOUT(HDC hdc,int x,int y,Ty&&... params)
	{
		std::wstringstream wss;
		(wss << ... << std::forward<Ty>(params));
		std::wstring wstr = wss.str();

		TextOut(hdc, x, y, wstr.c_str(), wstr.size());
	}
};

