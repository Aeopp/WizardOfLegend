#pragma once
#include "Scene.h"

class Scene_Boss : public Scene
{
public:
	~Scene_Boss()noexcept override;

	vec PlayerSpawnLocation = { 0,0 };

	void render(HDC hdc, std::pair<float, float> size_factor)override;
	void update(float dt) override;
	void initialize()override;
	void release() override;
};

