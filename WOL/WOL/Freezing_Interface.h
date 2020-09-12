#pragma once
#include "ICE_Blast.h"

class Freezing_Interface abstract
{
public:
	vec render_size = { 200,200 };
	static inline const float Percentage = 0.1;
	static inline const COLORREF EffectColor = RGB(127, 255, 255);
	
	static inline const float  Amplification_factor = 1.1f;
	
	void Freezing_update(float dt,std::weak_ptr<class collision_component> 
	_collision_component);
	/// <param name="location"></param>
	/// ī�޶� ��ǥ���� �� ������ ȭ�� ��ǥ�� �н����ּ���.
	void Freezing_render(HDC hdc, vec location);
	// ���� ���߽� �Ӽ����� �ߵ� Ȯ���� ���
	static bool probability_calculation(float _Percentage = Freezing_Interface::Percentage);
public:
	struct Freezing
	{
		static inline const float Duration = 3.0f;
		static inline const float AnimDelta = 0.15f;
		static inline const float InitAnimTime = 2.4f;
		static inline const vec PaintSize = { 200,250 };

		std::shared_ptr<class Bmp> GetImg();
		float AnimTick = AnimDelta;

		void Freez(vec msgLocation);
		float RemainTime = 0.f;
		int ColIdx = 0;
		inline bool IsFreezing()const&;
		bool update(float dt);
		// ī�޶� ��ǥ���� �� ���� ȭ����ǥ�� �Ѱ��ּ���.
		void render(HDC hdc, vec location, vec size);
	};
	
	Freezing _Freezing_Info;
};


bool Freezing_Interface::Freezing::IsFreezing() const&
{
	return RemainTime > 0;
};

