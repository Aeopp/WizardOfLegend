#pragma once
#include "object.h"
class BOTTOM_HOLE :
    public object
{
public:
	void initialize()override;
	Event update(float dt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	 // �׸� ��ġ�� ���ӽð��� �ݵ�� �˷��ּ��� .
	void late_initialize(vec InitLocation,float Duration);
	std::shared_ptr<class Bmp> sp_Bmp;



	vec PaintLocation;

	uint32_t get_layer_id()const& override;


	float Duration; 
	float ScaleX;
	float ScaleY;
};

