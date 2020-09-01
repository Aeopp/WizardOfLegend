#include "pch.h"
#include "Scene.h"
#include "object.h"
#include "object_mgr.h"
Scene::~Scene() noexcept
{
	release();
}

void Scene::update(float dt)
{
	std::erase_if(manage_objs, 		
	[](auto& Target) {if (!Target)return true; return Target->bDie; });
}

void Scene::release()
{
	for (auto& obj : manage_objs)
	{
		if (!obj)continue;
		obj->bDie = true;
	}
	manage_objs.clear();
}
