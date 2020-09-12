#include "pch.h"
#include "Freezing_Interface.h"
#include "collision_component.h"



void Freezing_Interface::Freezing_update(float dt, std::weak_ptr<collision_component>
_collision_component)
{
	_Freezing_Info.update(dt);
	
	if (_Freezing_Info.IsFreezing())
	{
		auto sp_collision = _collision_component.lock();
		if (!sp_collision)return;
		sp_collision->bSuperArmor = true;
	}
	else
	{
		auto sp_collision = _collision_component.lock();
		if (!sp_collision)return ;
		sp_collision->bSuperArmor = false;
	}
}

void Freezing_Interface::Freezing_render(HDC hdc, vec location)
{
	_Freezing_Info.render(hdc, location, Freez_size);
}
