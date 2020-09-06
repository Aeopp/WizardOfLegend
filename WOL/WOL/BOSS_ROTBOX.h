#pragma once
#include "object.h"
class BOSS_ROTBOX :
    public object
{
public:
	void initialize()override;
	Event update(float dt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	// ù ���� ��ġ ���� ���� ��ġ�ϱ���� �ɸ��� �ð� , ��ġ������ ������ ���ư� ������
	// ����ϱ� ���� Ÿ��
	void late_initialize(vec InitLocation, float InitDuration,
		std::weak_ptr<class object> wp_AttackTarget,vec Center,float RotationDuration);
	void HitTile(RECT rt)override;

	std::weak_ptr< class collision_component> _collision_component;
	std::shared_ptr<class Bmp> sp_Bmp;
	std::shared_ptr<class Bmp> sp_SOILBmp;



	
	float SOILAnimTick = 0;
	int SOILAnimRowDecrement = 0;

	vec RotationDir; 
	float DistanceOfCenter = 300;
	float CurrentAddAngle = 0;
	float RotationSpeed = 10.f;
	float RotationDuration;
	vec RotationCenter;
	
	vec InitLocation{};

	uint32_t get_layer_id()const& override;

	// ������ �������� ȿ��
	float FlyTimeInAddY = 50;

	// ����Ʈ �÷� ó���� �����ϰ� ������ ����
	int SOIL_NUM = 18;
	std::vector<std::pair<int32_t, vec>> SOIL_Cols;
	float SOIL_Duration = 2.5f;
	static inline const float SOIL_AnimDelta = 0.08f;
	int SOIL_PaintSizeX;
	int SOIL_PaintSizeY;
	float SOIL_ScaleX;
	float SOIL_ScaleY;

	float Duration = 20.f;
	float Speed;
	int PaintSizeX;
	int PaintSizeY;
	float ScaleX;
	float ScaleY;

	float InitDuration;

	bool bLaunch{ false };
};

