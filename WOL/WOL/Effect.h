#pragma once
#include "object.h"
#include "Color.h"

class Effect :
    public object
{
public:
	std::shared_ptr<class Bmp> EffectImg{};
	 Event update(float dt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	 void initialize()override;
public:
	 uint32_t get_layer_id()const& override;
public:
	
	// ��ƼŬ ����� ������ ����
	bool bDie{ false };
	int CurrentCol;
	float AnimDelta;

	// ������Ƽ�� �ʱ�ȭ ���ּ��� ===========================================
	COLORREF ColorKey = COLOR::MEGENTA();
	// �̹��� �׸� ��ġ (���ͱ���)
	int ImglocationX;
	int ImgLocationY;
	std::wstring ImgKey{};

	layer_type layer_ID;

	// �ִϸ��̼� �� ����
	int AnimColNum;
	// ����� �ִϸ��̼� �ο� �ε���
	int AnimRowIndex;

	// ����ð��� �������ּ���.
	float Duration;
	float AnimDuration;
	// �̹����� ���� ũ��
	int PaintSizeX;
	int PaintSizeY;
	// �̹����� ���� ������ (�����Ϸ� ����)
	float ScaleX;
	float ScaleY;

	void late_initialize(int ImgLocationX,int ImgLocationY,
	std::wstring ImgKey, layer_type layer_ID,int AnimColNum,
		int AnimRowIndex,float Duration,float AnimDuration,
		int PaintSizeX,int PaintSizeY,float ScaleX,float ScaleY);
};

