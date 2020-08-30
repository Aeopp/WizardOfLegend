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
	
	// 파티클 재생이 끝나면 삭제
	bool bDie{ false };
	int CurrentCol;
	float AnimDelta;

	// 프로퍼티들 초기화 해주세요 ===========================================
	COLORREF ColorKey = COLOR::MEGENTA();
	// 이미지 그릴 위치 (센터기준)
	int ImglocationX;
	int ImgLocationY;
	std::wstring ImgKey{};

	layer_type layer_ID;

	// 애니메이션 열 개수
	int AnimColNum;
	// 재생할 애니메이션 로우 인덱스
	int AnimRowIndex;

	// 재생시간을 설정해주세요.
	float Duration;
	float AnimDuration;
	// 이미지의 실제 크기
	int PaintSizeX;
	int PaintSizeY;
	// 이미지의 월드 사이즈 (스케일로 적용)
	float ScaleX;
	float ScaleY;

	void late_initialize(int ImgLocationX,int ImgLocationY,
	std::wstring ImgKey, layer_type layer_ID,int AnimColNum,
		int AnimRowIndex,float Duration,float AnimDuration,
		int PaintSizeX,int PaintSizeY,float ScaleX,float ScaleY);
};

