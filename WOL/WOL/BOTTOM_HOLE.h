#pragma once
#include "object.h"
class BOTTOM_HOLE :
    public object
{
public:
	void initialize()override;
	Event update(float dt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	 // 그릴 위치와 지속시간을 반드시 알려주세요 .
	void late_initialize(vec InitLocation,float Duration);
	std::shared_ptr<class Bmp> sp_Bmp;



	vec PaintLocation;

	uint32_t get_layer_id()const& override;


	float Duration; 
	float ScaleX;
	float ScaleY;
};

