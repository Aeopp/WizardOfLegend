#include "pch.h"
#include "UISkillIBarIcon.h"

#include "Bmp.h"
#include "UIInventory.h"
#include "Bmp_mgr.h"

#include "game.h"
#include "render_component.h"
#include "Color.h"

std::weak_ptr<class Bmp> UISkillIBarIcon::SkillBarBmpUpdate()
{
	switch (UIInventory::SlotInfoMap[CurrentSlotIdx]._Skill)
	{
	case ESkill::Normal:
		break;
	case ESkill::Dash:
		break;
	case ESkill::BLAST:
		return Bmp_mgr::instance().Find_Image_WP(L"ICE_BLAST_SKILLBAR.bmp");
		break;
	case ESkill::ARMOR:
		return  Bmp_mgr::instance().Find_Image_WP(L"GAIA_ARMOR_SKILLBAR.bmp");
		break;
	case ESkill::FIRE:
		return  Bmp_mgr::instance().Find_Image_WP(L"FIRE_DRAGON_SKILLBAR.bmp");
		break;
	case ESkill::CRYSTAL:
		return Bmp_mgr::instance().Find_Image_WP(L"ICE_KRYSTAL_SKILLBAR.bmp");
		break;
	case ESkill::BOOMERANG:
		return Bmp_mgr::instance().Find_Image_WP(L"BOOMERANG_SKILLBAR.bmp");
		break;
	default:
		break;
	}

	return {};
}

void UISkillIBarIcon::late_initialize(vec pos,std::wstring FileName)
{
	// 해상도 변경시 비율 조정을 위해서
	auto [x, y] = game::instance().size_factor();

	_render_component = render_component::LoadRenderComponent_SP(FileName.c_str(), FileName.c_str());
	_render_component->Default_Src_Paint_Size = vec{ 40 * x,40 * y };
	_render_component->_ColorKey = COLOR::MRGENTA();
	_render_component->_Img_src = RECT{ 0,0,54,54 };
	_render_component->_RenderDesc = ERender::Transparent;

	_transform->_location = std::move(pos); 

}

void UISkillIBarIcon::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!UIInventory::SlotInfoMap[CurrentSlotIdx].bAcquire)return;
	auto& Render = _render_component;
	Render->wp_Image = SkillBarBmpUpdate();
	auto sp_Img = Render->wp_Image.lock();
	if (!sp_Img)return;

	CoolTimeRefUpdate();
	if (!Max)return;
	if (!Current)return;
	float fCurrent = *Current;
	float fMax = *Max;

	float factor = fCurrent / fMax;

	vec v = Render->Dest_Loc = _transform->_location;
	v.x -= Render->Default_Src_Paint_Size.x / 2;
	v.y -= Render->Default_Src_Paint_Size.y / 2;

	vec WorldSize = Render->Default_Src_Paint_Size;

	vec ImgSize = {Render->_Img_src.right, Render->_Img_src.bottom};

	float re = 1 - factor;

	GdiTransparentBlt(hdc, v.x, v.y+WorldSize.y*re, Render->Default_Src_Paint_Size.x,
	/*r->Default_Src_Paint_Size.y*/0 + WorldSize.y * factor,
	sp_Img->Get_MemDC(), 0, 0+ImgSize.y*re, ImgSize.x, /*s.y*/0 + ImgSize.y*factor, RGB(255, 0, 255));
}

Event UISkillIBarIcon::update(float dt)
{
	return UIInfo::update(dt);
}

void UISkillIBarIcon::CoolTimeRefUpdate()
{
	auto sp_PlayerInfo = wp_PlayerInfo.lock();
	if (!sp_PlayerInfo)return;

	switch (UIInventory::SlotInfoMap[CurrentSlotIdx]._Skill)
	{
	case ESkill::Normal:
		break;
	case ESkill::Dash:
		break;
	case ESkill::BLAST:
		Current = &sp_PlayerInfo->SkillCurrentICEBlastCoolTime;
		Max = &sp_PlayerInfo->SkillICEBlastCoolTime;
		break;
	case ESkill::ARMOR:
		Current = &sp_PlayerInfo->SkillCurrentShieldCoolTime;
		Max = &sp_PlayerInfo->SkillShieldCoolTime;
		break;
	case ESkill::FIRE:
		Current = &sp_PlayerInfo->SkillCurrentBoomerangNum;
		Max = &sp_PlayerInfo->SkillBoomerangMaxNum;
		break;
	case ESkill::CRYSTAL:
		Current = &sp_PlayerInfo->SkillCurrentICECrystalCoolTime;
		Max = &sp_PlayerInfo->SkillICECrystalCoolTime;
		break;
	case ESkill::BOOMERANG:
		break;
	default:
		break;
	}

	return;

}
