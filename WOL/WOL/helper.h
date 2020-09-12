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


namespace std
{
	template<typename T>
	std::vector<T>& operator+=(std::vector<T>& Lhs, const std::vector<T>& Rhs)
	{
		Lhs.reserve(Lhs.size() + Rhs.size());                
		Lhs.insert(Lhs.end(), Rhs.begin(), Rhs.end());        
		return Lhs;                                        
	}
    // R-Value 레퍼런스로 패스했을 경우 주의
	template<typename T>
	std::vector<T>& operator+=(std::vector<T>& Lhs,std::vector<T>&& Rhs)
	{
		Lhs.reserve(Lhs.size() + Rhs.size());
		std::move(std::begin(Rhs), std::end(Rhs), std::back_inserter(Lhs));
		return Lhs;
	}
}
	

