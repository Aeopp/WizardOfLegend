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

	vec PlayerSpawnLocation = { 2564,2413};

	void player_set_up();
	void store_set_up(std::weak_ptr<class Player> _Player);
	~Scene_Stage()noexcept override;
private:
	void TriggerSetUp(std::weak_ptr<class Player> _Player);
};

