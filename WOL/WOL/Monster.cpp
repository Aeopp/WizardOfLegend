#include "pch.h"
#include "Monster.h"
#include "collision_mgr.h"
#include "object_mgr.h"
#include "timer.h"
#include "render_component.h"
#include "Color.h"
#include "Bmp_mgr.h"
#include "Effect.h"
#include "GoldEffect.h"



void Monster::CheckDirChangeImgFile()
{
	vec dir = _transform->_dir;
	float scala = dir.dot(vec{ 1,0 });


	if (scala > 0)
	{
		_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(RightAnimKey);
	}
	else
	{
		_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(LeftAnimKey);
	}
}


void Monster::initialize()
{
	actor::initialize();
	
	_collision_component_lower = collision_mgr::instance().insert(_ptr, collision_tag::EMonster, ERect);
	auto sp_collision_lower = _collision_component_lower.lock();

	if (!sp_collision_lower)return;

	collision_lower_correction = lower_correction;

	sp_collision_lower->_size = lower_size;
	sp_collision_lower->bSlide = true;
	sp_collision_lower->bHitEffect = false;

	if(_transform)
	_transform->_location = { 0.f,0.f };

	bAttacking = true;

	_render_component = render_component::LoadRenderComponent_SP
	(LeftAnimKey+L".bmp", LeftAnimKey);
	_render_component->Default_Src_Paint_Size = vec{ PaintSizeX,PaintSizeY };
	_render_component->Dest_Paint_Size = vec{ PaintSizeX * ScaleX,PaintSizeY * ScaleY};
	_render_component->_ColorKey = COLOR::MEGENTA();
	_render_component->_Img_src = RECT{ 0,0,PaintSizeX,PaintSizeY };
	_render_component->_Anim.SetAnimationClip(
		MyAnimInfo, MyAnimDuration);

	_Shadow.correction = shadow_correction;
	_Shadow.world_size_correction = ShadowWorldSizeCorrection;
	id = object::ID::monster;

};

Event Monster::update(float dt)
{
	Event _E = actor::update(dt);

	CheckDirChangeImgFile();
	              

	return _E;
};

void Monster::Hit(std::weak_ptr<object> _target)
{
	actor::Hit(_target);
	
}

void Monster::late_initialize(std::weak_ptr<class object> SetTarget, vec SetLocation)
{
	_AttackTarget = std::move(SetTarget);
	if(_transform)
	_transform->_location = std::move(SetLocation); 

	CardEffect(SummonCardImgKey);
}


/*late_initialize(int ImgLocationX, int ImgLocationY,
		std::wstring ImgKey, layer_type layer_ID, int AnimColNum,
		int AnimRowIndex, float Duration, float AnimDuration,
		int PaintSizeX, int PaintSizeY, float ScaleX, float ScaleY);*/
		// TOOD :: Scene Dependent Init 

void Monster::CardEffect(std::wstring ImageKey)
{
	vec  v = _transform->_location;

	object_mgr::instance().insert_object<Effect>(v.x,
		v.y,std::move(ImageKey),layer_type::EEffect,
		32,0,2.7f,2.7f,150,230,1.0f,1.0f);
}
;
