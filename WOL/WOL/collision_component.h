#pragma once
#include "collision_tag.h"

class object;

class collision_component
{
private:
	std::weak_ptr<class object> _owner{};
public:
	bool bCollision{ true };
	bool bRender{ true };

	void set_owner(std::weak_ptr<object> _owner)&;
	std::weak_ptr<object> get_owner()const { return _owner; }
	// ���и�
	bool bSlide{ false };
	// ������Ʈ �и�
	bool bCollisionSlideAnObject{ false };

	// ��ȣ�ۿ�ÿ� ����� ��Ʈ �÷�
	COLORREF HitColor;

	float CurrentCoolTime = 0.0f;

	void Hit(std::weak_ptr<class object> _target);
	figure_type _figure_type{ figure_type::ERect };

	bool bSuperArmor = false;
	bool bCollisionTargetPushFromForce{ false };
	// �ش� ������Ʈ�� Src �ϰ�� ��Ʈ ����Ʈ �����ų�� ����
	bool bHitEffect{ true };
	bool bTileHitEffect{ false }; 
	float fSlideFactor = 1.f;

	float PushForce{};
	vec _size;
	vec correction{};
	RECT make_rect();
	circle make_circle();
	vec make_center();

	collision_tag _Tag{};

	bool bDie{ false };
};

