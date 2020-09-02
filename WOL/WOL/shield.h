#pragma once
#include "actor.h"
class shield :
    public actor
{
public :
	static void DefenseMsg(vec loc);
	void initialize()override;
	Event update(float dt)override;
	std::weak_ptr<class collision_component>  _collision_component;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void Hit(std::weak_ptr<object> _target)override;
	void CalcIdx();
	uint32_t get_layer_id()const& override;
	vec HoleLocation{};
	void late_initialize(Transform _Transform);


	// 스프라이트 각 30도마다 준비되어 있음 .
	// =======================================================
		static inline int ShieldSpriteAngle = 30;
		static inline int  ShieldRotationSpeed = 75; 
		static inline double ShieldSpriteChangeDelta = (double)ShieldSpriteAngle / (double)ShieldRotationSpeed;
		double CurrentImgChangeRemainTime = ShieldSpriteChangeDelta;
	// ======================================================


	float _shield_distance{ 135.0f };
	float duration{};
	float DegreePerSecond{};
	float DegreeTick{};
	int ImgIndex{ 0 };
	float Angle{};
};

