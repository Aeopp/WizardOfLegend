#pragma once 
#include "object.h"
#include "event.h"

class ArcherArrow : public object
{
public:
	void launch(vec init,vec dir,int Row, std::shared_ptr<class Bmp> SetImg);
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	int RowIdx;
	std::weak_ptr<class collision_component> wp_collision{};
	std::shared_ptr<class Bmp>sp_Img;

	int PaintSizeX = 180;
	int PaintSizeY = 170;
	int ScaleX = 1;
	int ScaleY = 1;
	float speed = 400.f;

	Event update(float dt)override;
	void HitTile(RECT rt)override;
	void initialize();
	uint32_t get_layer_id()const& override;
	void Hit(std::weak_ptr<object> _target)override;
};