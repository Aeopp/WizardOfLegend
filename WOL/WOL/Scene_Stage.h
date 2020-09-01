#pragma once
#include "Scene.h"
class Scene_Stage :
    public Scene
{
public:
	void render(HDC hdc, std::pair<float, float> size_factor)override;
	void update(float dt) override;
	void initialize()override;
	void release() override ;

	vec PlayerSpawnLocation = { 600,600 };

	~Scene_Stage()noexcept override;
private:
};

