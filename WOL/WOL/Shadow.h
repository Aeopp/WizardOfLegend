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
	static inline std::map<EShadowState, std::tuple<float, float, float, float>> ShadowStates_Size
	{ 
							 // 페인트 사이즈 , 월드 사이즈
		{EShadowState::MIDDLE, {106,35,75,35} },
		{EShadowState::BIG_VER,{70,94,100,43}  },
		{EShadowState::NORMAL, {70,35,35,35}  },
		{EShadowState::BIG,    {117,35,50,80} } 
	};

	static void ShadowImgInit();
public:
	bool bShadow = true;
	std::shared_ptr<class Bmp> FindImgByState();
	void SetState(EShadowState _State) { CurrentShadowState = _State;  }
	void update();
	void initialize();
	void render(HDC hdc, vec camera_pos);
	std::weak_ptr<class object> _owner{};
	std::pair<float,float> world_size_correction{0,0};
	std::pair<float, float> correction{ 0,0 };
	EShadowState CurrentShadowState{EShadowState::NORMAL};
public:
	std::weak_ptr<class Bmp> wp_Big_shadow{};
	std::weak_ptr<class Bmp> wp_Big_shadow_ver{};
	std::weak_ptr<class Bmp> wp_Normal_shadow{};
	std::weak_ptr<class Bmp> wp_Middle_shadow{};
};

