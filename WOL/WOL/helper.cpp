#include "pch.h"
#include "helper.h"

#include "object_mgr.h"

#include "collision_mgr.h"

 void helper::MissMsg(vec Loation)
{
	vec randvec = math::RandVec();
	randvec.y = (abs(randvec.y));
	Loation.y -= 35;
	Loation.x += math::Rand<int>({ -40,+40 });

	object_mgr::instance().TextEffectMap[RGB(108, 32, 200)].
		push_back({ Loation ,vec{ 0,1 }*3,
			1.f,30,L"MISS" });

	collision_mgr::instance().HitEffectPush(Loation, 0.5f);
}
