#include "pch.h"
#include "render_component.h"
#include "Bmp.h"
#include "Bmp_mgr.h"
#include "game.h"

render_component::~render_component() noexcept
{
	static int render_destroy = 0;
	render_destroy++;
}

void render_component::release() noexcept
{

};

std::shared_ptr<render_component> render_component::LoadRenderComponent_SP(const std::wstring& _filename, const std::wstring& ImageKey)
{
	auto sp_render_comp = std::make_shared<render_component>();
	sp_render_comp->LoadBmp(_filename, ImageKey);
	return sp_render_comp;
};

void render_component::LoadBmp(const std::wstring& _filename, const std::wstring& ImageKey)
{
	wp_Image = Bmp_mgr::instance().Insert_Bmp(_filename, ImageKey);
};

void render_component::ChangeImg(std::weak_ptr<class Bmp> p_wp_Image)
{
	if (!p_wp_Image.expired())
	{
		wp_Image = std::move(p_wp_Image);
	}
}

void render_component::ChangeAnimDir(std::weak_ptr<class Bmp> p_wp_Image,float default_dt)
{
	// 만약 같은 방향이라면 그냥 아무것도 안함
	// 다른 방향이라면 델타 초기화 해야함
	auto p_sp_image = p_wp_Image.lock();
	auto _sp_image = wp_Image.lock();

	if (!p_sp_image)return;
	if (!_sp_image)wp_Image= p_wp_Image;

	if (p_sp_image != _sp_image)
	{
		_Anim.AnimDuration = default_dt;
		_Anim.ColIndex = 0;
		_Anim.CurDelta = default_dt;
		wp_Image = p_wp_Image;
	}
};

void render_component::SetAnimDelta(float Dt)
{
	_Anim.AnimDuration = Dt;
}
;

void render_component::Render(HDC CurrentHdc)
{
	auto sp_Image = wp_Image.lock();
	if (!sp_Image)return;
	
	const vec& dl = Dest_Loc;
	const vec& ps = Dest_Paint_Size;

	RECT CullingRect{ dl.x,dl.y,ps.x + dl.x,ps.y + dl.y };
	RECT _rt = game::instance().client_rect;

	if (bDebug)
	{
		static DWORD CurTime = GetTickCount();
		static int CullingObj = 0;
		static int RenderObj = 0;
		if (CurTime + 1000 < GetTickCount())
		{
			CurTime = GetTickCount();
			game::instance().debug_cul_obj_setup(CullingObj, RenderObj);
			CullingObj = 0;
			RenderObj = 0;
		}

		if (!math::rectVSrect(CullingRect, _rt))
		{
			++CullingObj;
			return;
		}
		else
			++RenderObj;
	}
	_Anim.update();

	HDC _BDC = sp_Image->Get_MemDC();

	const RECT& s = _Img_src;
	const vec& ds = Default_Src_Paint_Size;

	switch (_RenderDesc)
	{
	case Transparent:
		GdiTransparentBlt(CurrentHdc
			, dl.x,dl.y
			, ps.x, ps.y
			, _BDC
			, s.left + _Anim.ColIndex *ds.x, s.top + _Anim.RowIndex* ds.y
			, s.right  , s.bottom 
			, _ColorKey);
		break;
	default:
		break;
	}
};

