#pragma once
#include "UIInteraction.h"

class UIStart  :public UIInteraction
{
public:
	void initialize()override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	 void OnMouse(const vec hit_pos)override;
	 void Click(const vec hit_pos)override;
	Event update(float dt)override;
	float SoundTick = 0.1f;

	uint32_t get_layer_id() const&;

private:
	enum EMenu : uint8_t
	{
		Ready,
		Main,
		Option,
		Quit,
		Max,
	};
	EMenu eMenu= Ready;
	bool bStart = false;

	std::weak_ptr<class Bmp> wp_bmp_ready;
	std::weak_ptr<class Bmp> wp_bmp_option;
	std::weak_ptr<class Bmp> wp_bmp_main;
	std::weak_ptr<class Bmp> wp_bmp_quit;
};

