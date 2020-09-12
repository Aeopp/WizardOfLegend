#pragma once
#include "actor.h"
class ICE_Blast :
    public actor
{
public:
	void initialize()override;
	//void render(HDC hdc, vec camera_pos, vec size_factor)override;
	//void late_initialize(vec _location);
	Event update(float dt)override;
	std::weak_ptr<class collision_component>  _collision_component;
	void Hit(std::weak_ptr<object> _target) override;
	void HitTile(RECT _rt)override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	void IceEffectPlay();

	float ICE_EffectTick = 0.2f;
	float SoundTick = 0.5f;
	uint32_t get_layer_id()const& override;

	
	float ICE_Percentage = 0.1;
	
	float Duration;

	bool bSuccess{};

public :
	struct Freezing
	{
		static inline const float Duration = 3.0f;
		static inline const float AnimDelta = 0.15f;
		static inline const float InitAnimTime = 2.2f;
		static inline const vec PaintSize = { 200,250 };
	
		std::shared_ptr<class Bmp> GetImg();
		float AnimTick = AnimDelta;
		
		void Freez(vec msgLocation);
		float RemainTime = 0.f;
		int ColIdx = 0;
		inline bool IsFreezing()const&;
		bool update(float dt);
	    // 카메라 좌표까지 뺀 최종 화면좌표로 넘겨주세요.
		void render(HDC hdc, vec location, vec size);
	};
};


bool ICE_Blast::Freezing::IsFreezing() const&
{
	return RemainTime > 0;
};



