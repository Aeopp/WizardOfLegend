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

	void debug_cul_obj_setup(int cul_obj,int render_obj);

	int	print_cul_obj{};
	int print_render_obj{};


	RECT client_rect{};
	static constexpr int width = 1600;
	static constexpr int height = 900;
	// �⺻ �ػ󵵿��� ����ɽ� ��ȯ���� ����� ���ؼ� ������ �������ּ���.
	std::pair<float, float> size_factor();
public:
	HWND hWnd{};
};


