#pragma once

class Burning_Interface
{
public:
	static inline const float Percentage = 0.1;
	                        // 속성 상태일시 공격 받았을때 피해 증폭 계수
	static inline const float Amplification_factor = 1.3f;
	static inline const COLORREF EffectColor = RGB(255, 81, 79);
	void burning_update(float dt);
	/// <param name="location"></param>
	/// 카메라 좌표까지 뺀 이후의 화면 좌표로 패스해주세요.
	void Burning_render(HDC hdc, vec location);
	// 공격 적중시 속성공격 발동 확률을 계산
	static bool probability_calculation(float _Percentage =
	Burning_Interface::Percentage);
public:
	struct Burning
	{
		static inline const float Duration = 2.0f;
		static inline const float AnimDelta = 0.08f;
		static inline const vec PaintSize = { 70,70 };
		vec particle_range = {  25,75};
		// 파티클 개수
		int32_t ParticleNum = 10;
		// 파티클 렌더링 사이즈 xy : Min  Max  
		vec render_world_size_range= {  22 ,44 };
		struct Particle_Info
		{
			static Particle_Info MakeInfo(vec particle_range,vec render_world_size_range);
			// 타겟으로부터 상대적인 거리
			vec render_location{};
			vec render_world_size{};
			vec render_dir{};
			float AnimTick = Burning::AnimDelta;
			int ColIdx;
			int RowIdx;
		};
		
		std::vector<Particle_Info> _Particle_Infos;
		std::shared_ptr<class Bmp> GetImg();
	
		void burn(vec msgLocation);
		float RemainTime = 0.f;
		inline bool IsBurning()const&;
		bool update(float dt);
		// 카메라 좌표까지 뺀 최종 화면좌표로 넘겨주세요.
		void render(HDC hdc, vec location);
	};

	Burning _Burning_Info;
};

