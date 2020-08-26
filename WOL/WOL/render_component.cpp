#include "pch.h"
#include "render_component.h"
#include "Bmp.h"
#include "Bmp_mgr.h"

void render_component::release() noexcept
{

};

std::shared_ptr<render_component> render_component::MakeRenderComponent_SP(const std::wstring& _filename, const std::wstring& ImageKey)
{
	auto sp_render_comp = std::make_shared<render_component>();
	sp_render_comp->LoadBmp(_filename, ImageKey);
	return sp_render_comp;
};

void render_component::LoadBmp(const std::wstring& _filename, const std::wstring& ImageKey)
{
	wp_Image = Bmp_mgr::instance().Insert_Bmp(_filename, ImageKey);
};

void render_component::Render(HDC hDC)
{
	auto sp_Image = wp_Image.lock();

	if (!sp_Image)return;

	HDC _BDC = sp_Image->Get_MemDC();

	const RECT& s = _Img_src;
	const vec& dl = Dest_Loc;
	const vec& ps = Dest_Paint_Size;

	switch (_RenderDesc)
	{
	case Transparent:
		GdiTransparentBlt(hDC
			, dl.x,dl.y
			, ps.x, ps.y
			, _BDC
			, s.left, s.top
			, s.right, s.bottom
			, _ColorKey);
		break;
	default:
		break;
	}
};

