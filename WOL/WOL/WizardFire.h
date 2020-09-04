#pragma once
#include "object.h"
class WizardFire :
    public object
{
public:
	void Cast(vec init, vec dir, int Row, std::shared_ptr<class Bmp> SetImg);
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	int RowIdx;
	int ColIdx;
	
	static inline float AnimDuration = 0.8f;
	static inline float AnimBetWeenDelta = AnimDuration / 6.f;
	static inline int AnimColNum = 6;

	float CurrentAnimDelta = AnimBetWeenDelta;

	std::weak_ptr<class collision_component> wp_collision{};
	std::shared_ptr<class Bmp>sp_Img;
	std::weak_ptr<class object>wp_Target{};

	int PaintSizeX = 180;
	int PaintSizeY = 170;
	int ScaleX = 0.8;
	int ScaleY = 0.8;
	float speed = 400.f;

	int CalcRowFromAngle(vec dir);

	Event update(float dt)override;
	void HitTile(RECT rt)override;
	void initialize();
	uint32_t get_layer_id()const& override;
	void Hit(std::weak_ptr<object> _target)override;

	
};
