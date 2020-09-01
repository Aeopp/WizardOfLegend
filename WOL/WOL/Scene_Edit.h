#include "Scene.h"
#include "Tile.h"

class Scene_Edit :
    public Scene
{
public:
	void render(HDC hdc, std::pair<float, float> size_factor)override;
	void update(float dt) override;
	void initialize()override;
	void release() override;

	vec controll_pos{ 0,0 };
	float scrool_speed{ 50.f };
	vec select_image_start_pos{ 300,300 };

	std::pair<vec, vec> Tile_image_start_pos{ {73,82},{515,82} };
	int ImageMiddleX = 458;

	ETileSelect CurrentSelectImage{};

	bool bSelect{ false };
	std::pair<int, int> CurrentTileIndex{};
	std::pair<int, int> CurrentImageStartPos{};

	uint32_t CurrentTileImgSizeX{};
	uint32_t CurrentTileImgSizeY{};

	uint32_t CurrentTileWorldSizeX{};
	uint32_t CurrentTileWorldSizeY{};


	void Input_Check_Scroll();
	void Camera_Scroll();
	~Scene_Edit()noexcept override;
};
