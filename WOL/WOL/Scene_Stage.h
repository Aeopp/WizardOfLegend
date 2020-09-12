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
	std::shared_ptr<class object> Stage_1(std::weak_ptr<class Player> _Player);
	std::shared_ptr<class object> Stage_2(std::weak_ptr<class Player> _Player);
	std::shared_ptr<class object> Stage_3(std::weak_ptr<class Player> _Player);
	std::shared_ptr<class object> Stage_4(std::weak_ptr<class Player> _Player);
	std::shared_ptr<class object> Stage_5(std::weak_ptr<class Player> _Player);
	std::shared_ptr<class object> Stage_6(std::weak_ptr<class Player> _Player);
	std::shared_ptr<class object> Stage_7(std::weak_ptr<class Player> _Player);
	
	void TriggerSetUp(std::weak_ptr<class Player> _Player);
};

