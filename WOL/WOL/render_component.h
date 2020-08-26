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
	COLORREF _ColorKey{RGB(255,0,255) };

	// 이미지의 파일 사이즈와 동일하게 설정해주세요.
	RECT _Img_src{};
	// 이미지를 그릴 대상의 좌표와 사이즈를 지정해주세요.
	vec Dest_Loc{};
	vec Dest_Paint_Size{};
	vec Default_Dest_Paint_Size{};

	ERender _RenderDesc{ Transparent };

	void Render(HDC hDC);
};

