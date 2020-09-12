#pragma once

class Burning_Interface
{
public:
	static inline const float Percentage = 0.1;
	                        // �Ӽ� �����Ͻ� ���� �޾����� ���� ���� ���
	static inline const float Amplification_factor = 1.3f;
	static inline const COLORREF EffectColor = RGB(255, 81, 79);
	void burning_update(float dt);
	/// <param name="location"></param>
	/// ī�޶� ��ǥ���� �� ������ ȭ�� ��ǥ�� �н����ּ���.
	void Burning_render(HDC hdc, vec location);
	// ���� ���߽� �Ӽ����� �ߵ� Ȯ���� ���
	static bool probability_calculation(float _Percentage =
	Burning_Interface::Percentage);
public:
	struct Burning
	{
		static inline const float Duration = 2.0f;
		static inline const float AnimDelta = 0.08f;
		static inline const vec PaintSize = { 70,70 };
		vec particle_range = {  25,75};
		// ��ƼŬ ����
		int32_t ParticleNum = 10;
		// ��ƼŬ ������ ������ xy : Min  Max  
		vec render_world_size_range= {  22 ,44 };
		struct Particle_Info
		{
			static Particle_Info MakeInfo(vec particle_range,vec render_world_size_range);
			// Ÿ�����κ��� ������� �Ÿ�
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
		// ī�޶� ��ǥ���� �� ���� ȭ����ǥ�� �Ѱ��ּ���.
		void render(HDC hdc, vec location);
	};

	Burning _Burning_Info;
};

