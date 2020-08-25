#include "pch.h"
#include "collision_mgr.h"
#include "collision_component.h"

void collision_mgr::render(HDC hdc)
{
	for (auto& [_, collision_list] : _collisions)
	{
		for (auto& collision : collision_list)
		{
			
		}
	}
 }

void collision_mgr::update()
{
}

void collision_mgr::release()
{
	_collisions.clear();

}
