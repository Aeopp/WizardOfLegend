#pragma once
#include "object.h"

class BOSS_SKILL :public object
{
public:
	void initialize()override;
	Event update(float dt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void HitTile(RECT rt)override;
	// ù ���� ��ġ ���� ���� ��ġ�ϱ���� �ɸ��� �ð� , ��ġ������ ������ ���ư� ������
	// ����ϱ� ���� Ÿ��
	void late_initialize(vec InitLocation);
	std::weak_ptr< class collision_component> _collision_component;
	std::shared_ptr<class Bmp> sp_Bmp;
	std::shared_ptr<class Bmp> sp_SOILBmp;

	float Tick = 0;
	float fFloationgY = 0;
	// �� ����Ʈ�� ������ �÷����� ROW ���ҽ�Ű�� ����
	int SOILEffectNum = 8;
	std::vector<std::pair<int,vec>> MySOILColIdxs;
	int CurrentSOILRow = 6;
	static inline const float SOIL_AnimDelta = 0.07f;
	float CurrentSOIL_AnimDelta = SOIL_AnimDelta; 
	uint32_t get_layer_id()const& override;

	static inline const float PillarAnimDelta = 0.1f;
	float CurrentPillarAnimDelta = PillarAnimDelta; 
	int CurrentPillarColIdx = 0;
	// ����Ʈ �÷� ó���� �����ϰ� ������ ����
	
	float Pillar_ScaleX;
	float Pillar_ScaleY;
	float SOIL_ScaleX;
	float SOIL_ScaleY;

	float Duration = 10.f;
};

