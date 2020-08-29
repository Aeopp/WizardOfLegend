#pragma once
#include <memory>

enum class EShadowState : uint8_t
{
	MIDDLE,
	NORMAL,
	BIG_VER,
	BIG,
	MAX,
};
// 오브젝트가 생성한 이후에 오너와 월드 사이즈를 반드시 설정해주세요.
class Shadow
{
public:
	static inline std::map<EShadowState, std::tuple<int,int,int,int>> ShadowStates_Size
	{ 
		{EShadowState::MIDDLE, {106,35,75,50} },
		{EShadowState::BIG_VER,{70,94,100,50}  },
		{EShadowState::NORMAL, {70,35,30,30}  },
		{EShadowState::BIG,    {117,35,50,100} } 
	};

	static void ShadowImgInit();
public:
	std::shared_ptr<class Bmp> FindImgByState();
	void SetState(EShadowState _State) { CurrentShadowState = _State;  }
	void update();
	void initialize();
	void render(HDC hdc, vec camera_pos);
	std::weak_ptr<class actor> _owner{};
	std::pair<int,int> world_size_correction{0,0};
	std::pair<int, int> correction{ 0,0 };
	EShadowState CurrentShadowState{EShadowState::NORMAL};
public:
	std::weak_ptr<class Bmp> wp_Big_shadow{};
	std::weak_ptr<class Bmp> wp_Big_shadow_ver{};
	std::weak_ptr<class Bmp> wp_Normal_shadow{};
	std::weak_ptr<class Bmp> wp_Middle_shadow{};
};

