#pragma once
#include "pch.h"
#include "singleton_interface.h"

class game : public singleton_interface<game>
{
public:
	friend class  std::shared_ptr<game>;
public:
	void render();
	void update();
	void initialize();
	void release();
	void late_update();

	void debug_cul_obj_setup(int cul_obj, int render_obj);

	int	print_cul_obj{};
	int print_render_obj{};


	std::shared_ptr<class player_info> _player_info{};

	static inline RECT client_rect{};
	static constexpr int width = 1600;
	static constexpr int height = 900;

	// 타일의 월드 사이즈를 지정합니다.
	static constexpr int TileWorldX{ 53 };
	static constexpr int TileWorldY{ 52 };
	// 타일의 이미지 사이즈를 지정합니다.
	static constexpr int TileImgX{ 53 };
	static constexpr int TileImgY{ 52 };
	// 타일의 개수를 지정합니다
	static constexpr int TileNumX{ 100 };
	static constexpr int TileNumY{ 100 };
	// 기본 해상도에서 변경될시 반환값에 사이즈를 곱해서 비율을 유지해주세요.
	std::pair<float, float> size_factor();
	HDC hDC;
public:
	static inline HWND hWnd{};
	static inline  HDC CurrentHdc{};
};


