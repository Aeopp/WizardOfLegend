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
	float scrool_speed{ 100.f };
	vec select_image_start_pos{ 300,300 };

	std::pair<vec, vec> Tile_image_start_pos{ {73,82},{515,82} };
	int ImageMiddleX = 458;

	bool bSelect{ false };
	std::pair<int, int> CurrentTileIndex{};
	std::pair<int, int> CurrentImageStartPos{};

	static inline std::map<ETileSelect, std::pair<std::weak_ptr<class Bmp>,std::wstring>> ImageSelectMap{};
	ETileSelect CurrentSelectImage{};

	void Input_Check_Scroll();
	void Camera_Scroll();
	~Scene_Edit()noexcept override;
};
