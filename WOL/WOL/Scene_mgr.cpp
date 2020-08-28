#include "pch.h"
#include "Scene_mgr.h"
#include "Scene.h"
#include "Scene_Boss.h"
#include "Scene_Stage.h"
#include "Scene_Start.h"
#include "Scene_End.h"
#include "Scene_Edit.h"

void Scene_mgr::Scene_Change(ESceneID _ESceneID)
{
	_CurScene = _ESceneID;
	if (_PreScene != _CurScene)
	{
		if (sp_scene) sp_scene.reset();

		switch (_CurScene)
		{
		case ESceneID::EStart:
			sp_scene = std::make_shared<Scene_Start>();
			break;
		case ESceneID::EStage:
			sp_scene = std::make_shared<Scene_Stage>();
			break;
		case ESceneID::EBoss:
			sp_scene = std::make_shared<Scene_Boss>();
			break;
		case ESceneID::EEnd:
			sp_scene = std::make_shared<Scene_End>();
			break;
		case ESceneID::EEdit:
			sp_scene = std::make_shared<Scene_Edit>();
			break;
		case ESceneID::ENone:
			break;
		default:
			break;
		}
		sp_scene->initialize();
		_PreScene = _CurScene;
	}
}

void Scene_mgr::render(HDC hdc, std::pair<float, float> size_factor)
{
	if (!sp_scene)return;

	sp_scene->render(hdc,size_factor);
}

void Scene_mgr::update(float dt)
{

	if (!sp_scene)return;
	sp_scene->update(dt);
}

void Scene_mgr::initialize()
{

	if (!sp_scene)return;
	sp_scene->initialize();
}

void Scene_mgr::release()
{
	if (!sp_scene)return;
	sp_scene->release();
}
