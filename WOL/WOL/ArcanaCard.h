#pragma once
#include "object.h"
#include "ESkillBarIcon.h"


class ArcanaCard :
    public object
{
public :
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void initialize()override;
	void late_initialize(vec Location, ESkill SetSkill, std::wstring ImgKey);
	void Hit(std::weak_ptr<object> _target)override;
private:
	ESkill _Skill;
	std::shared_ptr<class Bmp> sp_Img;
	std::weak_ptr<class collision_component> _collision_component;
};

