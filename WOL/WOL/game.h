#pragma once
#include "pch.h"
#include "singleton_interface.h"

class game : public singleton_interface<game>
{
public:
	friend class  std::shared_ptr<game>;
public:
	void render(HDC hdc)override;
	void update()override;
	void initialize()override;
	void release()override;
	void late_update();

	RECT client_rect{};
public:
	HWND hWnd{};
};


