#include "pch.h"
#include "SwordManAttack.h"
#include "Bmp_mgr.h"
#include "Bmp.h"


void SwordManAttack::EffectStart(float duration, vec dir, vec location)
{
	_Dir = math::checkDir(dir);
	switch (_Dir)
	{
	case math::EDir::left:
		AnimRowIndex = 0;
		break;
	case math::EDir::right:
		AnimRowIndex = 1;
		break;
	case math::EDir::up:
		AnimRowIndex = 2;
		break;
	case math::EDir::down:
		AnimRowIndex = 4;
		break;
	default:
		break;
	}
	ImgLocationX = location.x;
	ImgLocationY = location.y;
	bRender = true; 

	this->Duration = duration;
};

Event SwordManAttack::update(float dt)
{
	Event _event = object::update(dt);

	Duration -= dt;
	if (Duration < 0) {
		bRender = true; 
	};

	AnimDelta -= dt;
	if (AnimDelta < 0)
	{
		AnimDelta = (float)AnimDuration / ((float)AnimColNum *2);
		++CurrentCol;
		if (CurrentCol >= AnimColNum)
		{
			switch (_Dir)
			{
			case math::EDir::left:
				break;
			case math::EDir::right:
				break;
			case math::EDir::up:
				AnimRowIndex++;
				break;
			case math::EDir::down:
				AnimRowIndex++;
				break;
			default:
				break;
			}
			AnimRowIndex++;
			CurrentCol = 0;
		}
	}
	return _event;
}

void SwordManAttack::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!bRender)return;

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
};

void SwordManAttack::initialize()
{
	object::initialize();
}
