#pragma once
#include <sstream>
#include <chrono>  
#include <ctime>  
#include <sstream> 
#include <iomanip>
#include <string>

#define _CRT_SECURE_NO_WARNINGS



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
	static std::wstring return_current_time_and_date()
	{
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
		return to_wstring(ss.str());
	}

	static std::wstring to_wstring(const std::string& str)
	{
		return std::wstring(std::begin(str), std::end(str));
	};

	static void MissMsg(vec Loation);;
};

