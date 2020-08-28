#include "pch.h"
#include "Scene.h"
#include "object.h"

Scene::~Scene() noexcept
{
	release();
}

void Scene::release()
{
	for (auto& target : manage_objs)
	{
		if (!target)continue ;
		target->bDie = true;
	}
	manage_objs.clear();
}
