#pragma once
#include "UIInteraction.h"

// 로케이션 설정
// 이미지 BMP 설정

class UISlot :
    public UIInteraction
{
public:
	void initialize()override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void OnMouse(const vec hit_pos)override;
	void Click(const vec hit_pos)override;
	Event update(float dt)override;
	uint32_t get_layer_id() const&;
	float SoundTick = 0.1f;

	void SetUp(vec location, std::wstring ImgKey);
public:
};

