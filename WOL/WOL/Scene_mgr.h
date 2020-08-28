#pragma once
#include "pch.h"
#include "singleton_interface.h"
#include "ESceneID.h"

class Scene_mgr : public  singleton_interface<Scene_mgr>
{	
public:
	void Scene_Change(ESceneID _ESceneID);

	void render(HDC hdc, std::pair<float, float> size_factor);
	void update(float dt);
	void initialize() ;
	void release() ;
private:
	std::shared_ptr<class Scene> sp_scene{};
	ESceneID _PreScene{ ESceneID::ENone };
	ESceneID _CurScene{ ESceneID::ENone };
};

