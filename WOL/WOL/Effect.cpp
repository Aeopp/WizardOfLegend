#include "pch.h"
#include "Effect.h"
#include "Bmp_mgr.h"
#include "Bmp.h"


Event Effect::update(float dt)
{
    Event _event = object::update(dt);
  
	Duration -= dt;
	AnimDelta -= dt;
	if (Duration < 0) {
		bDie = true;
		return Event::Die;
	}

	if (AnimDelta < 0)
	{
		AnimDelta = (float)AnimDuration / (float)AnimColNum;
		++CurrentCol;
		if (CurrentCol >= AnimColNum)
		{
			CurrentCol = 0;
		}
	}
    return _event; 
}

void Effect::render(HDC hdc, vec camera_pos, vec size_factor)
{
    object::render(hdc, camera_pos, size_factor);

	if (!EffectImg)return;

	//월드 사이즈
	int wsx = (PaintSizeX * ScaleX);
	int wsy = (PaintSizeY * ScaleY);

	vec  v = { ImgLocationX,ImgLocationY };
	v -= camera_pos;

	GdiTransparentBlt(hdc, v.x - wsx / 2, v.y - wsy / 2,
		wsx, wsy, EffectImg->Get_MemDC(), PaintSizeX * CurrentCol, PaintSizeY * AnimRowIndex
		, PaintSizeX, PaintSizeY, ColorKey);
}

void Effect::initialize()
{
    object::initialize();
}

uint32_t Effect::get_layer_id() const&
{
	return this->layer_ID;
}

void Effect::late_initialize(
	int ImgLocationX, int ImgLocationY, 
	std::wstring ImgKey, layer_type layer_ID,
	int AnimColNum, int AnimRowIndex, float Duration,
	float AnimDuration, int PaintSizeX, int PaintSizeY,
	float ScaleX, float ScaleY)
{
	_transform->_location.x = ImgLocationX;
	_transform->_location.y = ImgLocationY;
	
	this->ImgLocationX = ImgLocationX;
	this->ImgLocationY = ImgLocationY;
	this->ImgKey = ImgKey;
	this->layer_ID = layer_ID;
	this->AnimColNum = AnimColNum;
	this->AnimRowIndex = AnimRowIndex;
	this->Duration = Duration;
	this->AnimDuration = AnimDuration;
	this->PaintSizeX = PaintSizeX;
	this->PaintSizeY = PaintSizeY;
	this->ScaleX = ScaleX;
	this->ScaleY = ScaleY;

	AnimDelta = (float)AnimDuration / (float)AnimColNum;

	EffectImg = Bmp_mgr::instance().Find_Image_SP(ImgKey);
}
