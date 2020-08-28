#pragma once
#include "Scene.h"
class Scene_End :
    public Scene
{
public:
	~Scene_End()noexcept override;

	void render(HDC hdc, std::pair<float, float> size_factor)override;
	void update(float dt) override;
	void initialize()override;
	void release() override;
};

