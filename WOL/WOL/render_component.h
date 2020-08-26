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
	//                   전체경로가 아닌 파일 이름만 입력해주세요.
	//   전체경로를 컨트롤 하려면 매니저의 디폴트 path 를 수정해주세요 . 
	void LoadBmp(const std::wstring& _filename,const std::wstring& ImageKey);
	std::weak_ptr<class Bmp> wp_Image{};
	COLORREF _ColorKey{ RGB(255,0,255) };
	RECT _Img_src{};
	RECT _Img_Dest{};
	ERender _RenderDesc{ Transparent };

	void Render(HDC hDC);
};

