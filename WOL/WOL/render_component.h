#pragma once
#include "pch.h"

enum ERender : uint8_t
{
	Transparent,
};
class render_component
{
public:
	void release()noexcept;
	static std::shared_ptr<render_component> MakeRenderComponent_SP(
	const std::wstring& _filename, const std::wstring& ImageKey);
	//                   ��ü��ΰ� �ƴ� ���� �̸��� �Է����ּ���.
	//   ��ü��θ� ��Ʈ�� �Ϸ��� �Ŵ����� ����Ʈ path �� �������ּ��� . 
	void LoadBmp(const std::wstring& _filename,const std::wstring& ImageKey);
	std::weak_ptr<class Bmp> wp_Image{};
	COLORREF _ColorKey{ RGB(255,0,255) };
	RECT _Img_src{};
	RECT _Img_Dest{};
	ERender _RenderDesc{ Transparent };

	void Render(HDC hDC);
};

