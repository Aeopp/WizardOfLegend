#pragma once
#include "actor.h"
class shield :
    public actor
{
public :
	void initialize()override;
	Event update(float dt)override;
	std::weak_ptr<class collision_component>  _collision_component;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void CalcIdx();
	uint32_t get_layer_id()const& override;
	vec HoleLocation{};


	bool bHoleRender{ true };
	float HoleDuration{};
	float _shield_distance{ 150.0f };
	float duration{};
	float DegreePerSecond{};
	float DegreeTick{};
	int ImgIndex{ 0 };
	float Angle{};
};

