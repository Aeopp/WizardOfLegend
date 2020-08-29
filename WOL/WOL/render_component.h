#pragma once
#include "pch.h"
#include "Anim.h"
#include "timer.h"

enum ERender : uint8_t
{
	Transparent,
};

class render_component
{
public:
	~render_component()noexcept;

	void release()noexcept;
	static std::shared_ptr<render_component> LoadRenderComponent_SP(
	const std::wstring& _filename, const std::wstring& ImageKey);

	//                   ��ü��ΰ� �ƴ� ���� �̸��� �Է����ּ���.
	//   ��ü��θ� ��Ʈ�� �Ϸ��� �Ŵ����� ����Ʈ path �� �������ּ��� . 
	void LoadBmp(const std::wstring& _filename,const std::wstring& ImageKey);
	std::weak_ptr<class Bmp> wp_Image{};
	COLORREF _ColorKey{RGB(255,0,255) };

	template<typename T>

	void ChangeAnim(T AnimPlay, float Duration ,T DefaultClip = {}, std::weak_ptr<class Bmp> p_wp_Image = {});
	template<typename T>
	void ChangeUnstoppableAnim(T AnimPlay, float Duration, T DefaultClip = {}, std::weak_ptr<class Bmp> p_wp_Image = {});


	void ChangeAnimDir(std::weak_ptr<class Bmp>p_wp_Image, float default_dt);

	void SetAnimDelta(float Dt);
	// �̹����� ���� ������� �����ϰ� �������ּ���.
	RECT _Img_src{};
	// �̹����� �׸� ����� ��ǥ�� ����� �������ּ���.
	vec Dest_Loc{};
	vec Dest_Paint_Size{};
	vec Default_Src_Paint_Size{};

	Anim _Anim{};

	ERender _RenderDesc{ Transparent };

	void Render(HDC CurrentHdc);
};

template<typename T>
void render_component::ChangeUnstoppableAnim(T AnimPlay, float Duration, T DefaultClip, std::weak_ptr<class Bmp> p_wp_Image)
{
	if (!p_wp_Image.expired())
	{
		wp_Image = std::move(p_wp_Image);
	}
	
	_Anim.AnimPlay((int)AnimPlay, Duration);
	_Anim.SetDefaultClip((int)DefaultClip);
	_Anim.bChangeable = false;
	Timer::instance().event_regist(time_event::EOnce, Duration, [&bChangeAnim = _Anim.bChangeable]()->bool{bChangeAnim = true; return true; });
}

template<typename T>
void render_component::ChangeAnim(T AnimPlay, float Duration,T DefaultClip,std::weak_ptr<class Bmp> p_wp_Image)
{
	if (!p_wp_Image.expired())
	{
		wp_Image = std::move(p_wp_Image);
	}

	_Anim.AnimPlay((int)AnimPlay, Duration);
	_Anim.SetDefaultClip((int)DefaultClip);
}

