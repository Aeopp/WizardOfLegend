#pragma once
#include "pch.h"
#include "singleton_interface.h"

class game : public singleton_interface<game>
{
public:
	friend class  std::shared_ptr<game>;
public:
	void render(HDC hdc);
	void update();
	void initialize();
	void release();
	void late_update();

	RECT client_rect{};
	static constexpr int width = 1600;
	static constexpr int height = 900;
	// 기본 해상도에서 변경될시 반환값에 사이즈를 곱해서 비율을 유지해주세요.
	std::pair<float, float> size_factor();
public:
	HWND hWnd{};
};


