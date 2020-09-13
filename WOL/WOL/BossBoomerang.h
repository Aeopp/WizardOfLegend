#pragma once
#include "Boomerang.h"
#include "object.h"
class BossBoomerang :
    public object
{
public:
	void initialize()override;
	Event update(float dt)override;
	std::weak_ptr<class collision_component>  _collision_component;
	std::weak_ptr<class Bmp> wp_ResetImg{};
	std::weak_ptr<class Bmp> wp_RotImg{};
	std::weak_ptr<class Bmp> wp_Image{};

	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	uint32_t get_layer_id()const& override;
	void Hit(std::weak_ptr<object> _target)override;
	void HitTile(RECT TileRt)override;

	std::array<POINT, 3> RotPts;

	float InitTime = 1.f;
	bool bLaunch = false;
	
	void SetUp(std::weak_ptr<class object> wp_AttackTarget,vec InitLocation,vec InitDir,float InitTime=1);
	void CalcImgAngle(float RotationImgDegree);
	virtual ~BossBoomerang()noexcept;
public:
	float Duration;
	float AnimSpeed;
	int AnimRowIdx;
	int CurrentColIdx;
	float AnimDelta;
	float CurrentAnimRemainTime;
	float HitTileEffectRemainTime = Boomerang::HitTileEffectTick;
private:
	std::weak_ptr<class object> wp_Target{};
	float ScaleX ;
	float  ScaleY;
	int PaintSizeX;
	int PaintSizeY;
	float _speed ;
	float ImgAngle;
	float RotationSpeed;
	float HomingRotationCurrentSpeed ;
	float HomingRotationacceleration;
};

