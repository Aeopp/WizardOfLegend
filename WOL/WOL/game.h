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

	// Ÿ���� ���� ����� �����մϴ�.
	static constexpr int TileWorldX{ 53 };
	static constexpr int TileWorldY{ 52 };
	// Ÿ���� �̹��� ����� �����մϴ�.
	static constexpr int TileImgX{ 53 };
	static constexpr int TileImgY{ 52 };
	// Ÿ���� ������ �����մϴ�
	static constexpr int TileNumX{ 100 };
	static constexpr int TileNumY{ 100 };
	// �⺻ �ػ󵵿��� ����ɽ� ��ȯ���� ����� ���ؼ� ������ �������ּ���.
	std::pair<float, float> size_factor();
	HDC hDC;
public:
	static inline HWND hWnd{};
	static inline  HDC CurrentHdc{};
};


