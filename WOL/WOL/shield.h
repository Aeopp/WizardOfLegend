#pragma once
#include "actor.h"
class shield :
    public actor
{
public :
	static void DefenseMsg(vec loc);
	void initialize()override;
	Event update(float dt)override;
	std::weak_ptr<class collision_component>  _collision_component_lower;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void Hit(std::weak_ptr<object> _target)override;
	void CalcIdx();
	uint32_t get_layer_id()const& override;
	vec HoleLocation{};

	void late_initialize(Transform _Transform);

	float _shield_distance{ 135.0f };
	float duration{};
	float DegreePerSecond{};
	float DegreeTick{};
	int ImgIndex{ 0 };
	float Angle{};
};

