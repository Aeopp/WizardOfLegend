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

	~Scene_Stage()noexcept override;
private:
	
};

