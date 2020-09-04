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
	// 벽밀림
	bool bSlide{ false };
	// 오브젝트 밀림
	bool bCollisionSlideAnObject{ false };

	// 상호작용시에 출력할 폰트 컬러
	COLORREF HitColor;

	float CurrentCoolTime = 0.0f;

	void Hit(std::weak_ptr<class object> _target);
	figure_type _figure_type{ figure_type::ERect };

	bool bSuperArmor = false;
	bool bCollisionTargetPushFromForce{ false };
	// 해당 컴포넌트가 Src 일경우 히트 이펙트 재생시킬지 여부
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

