#pragma once
#include "Scene.h"

class Scene_Middle_Boss :
    public Scene
{
public:
	~Scene_Middle_Boss()noexcept override;

	vec PlayerSpawnLocation = { 2400,4728};

	void render(HDC hdc, std::pair<float, float> size_factor)override;
	void update(float dt) override;
	void initialize()override;
	void release() override;
};

