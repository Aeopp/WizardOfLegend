#include "pch.h"
#include "shield.h"
#include "collision_mgr.h"
#include "render_component.h"
#include "Color.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "Effect.h"
#include "object_mgr.h"
#include "sound_mgr.h"


void shield::DefenseMsg(vec loc)
{
	vec randvec = math::RandVec();
	randvec.y = (abs(randvec.y));
	vec v = std::move(loc);
	v.y -= 35;
	//	v.x += math::Rand<int>({ -40,+40 });

	object_mgr::instance().TextEffectMap[RGB(255, 248, 127)].
		push_back({ v,vec{0,1}*3,2.f,17,L"Defense!" });
}

void shield::initialize()
{
	actor::initialize();

	// Shadow SetUp
	{
		_Shadow.bShadow = false;
		_Shadow.correction = { 0,60 };
		_Shadow.world_size_correction = { -20,0 };
		_Shadow.CurrentShadowState = EShadowState::MIDDLE;
	};

	ObjectTag = object::Tag::player_shield;

	bAttacking = true;
	Attack = { 0,10 };

	UniqueID = EObjUniqueID::GAIR_ARMOR;
}

Event shield::update(float dt)
{
	Event _E =  actor::update(dt);
	duration -= dt;
	if (duration < 0)return Event::Die;

	auto Owner = _owner.lock();
	if (!Owner) return Event::Die;

	CurrentImgChangeRemainTime -= dt;
	if (CurrentImgChangeRemainTime < 0)
	{
		CurrentImgChangeRemainTime = ShieldSpriteChangeDelta;
		++_render_component->_Anim.ColIndex; 
		_render_component->_Anim.ColIndex %= 12;
		/*CalcIdx();*/
	}
	
	vec w = Owner->_transform->_location;

	vec& r = _transform->_dir;
	Angle += ShieldRotationSpeed * dt;
	if (Angle >= 360)
	{
		Angle = 0 + (360 - Angle);
	}
	r = math::rotation_dir_to_add_angle(r, ShieldRotationSpeed *dt);

	_transform->_location = w + r * _shield_distance;

	return _E;
}


void shield::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!_render_component) return;
	_render_component->Dest_Loc =
	_transform->_location - camera_pos - (_render_component->Dest_Paint_Size * 0.5);

	_Shadow.render(hdc, camera_pos);

	auto sp_Image = _render_component->wp_Image.lock();
	if (!sp_Image)return;

	switch (_render_component->_RenderDesc)
	{
	case Transparent:
		GdiTransparentBlt(hdc
			, _render_component->Dest_Loc.x,
			_render_component->Dest_Loc.y
			, _render_component->Dest_Paint_Size.x, 
			_render_component->Dest_Paint_Size.y

			, sp_Image->Get_MemDC()

			, _render_component->_Img_src.left + 
			_render_component->_Anim.ColIndex *
			_render_component->Default_Src_Paint_Size.x,
			_render_component->_Img_src.top
			, _render_component->_Img_src.right,
			_render_component->_Img_src.bottom
			, _render_component->_ColorKey);
		break;
	default:
		break;
	}
}

void shield::Hit(std::weak_ptr<object> _target)
{
	object::Hit(_target);
	//auto sp_target = _target.lock();
	//if (!sp_target)return;

}

void shield::CalcIdx()
{
	Angle = math::Angle360conversion(Angle);

	int Idx = (Angle / 30) - 9;
	if (Idx < 0) Idx += 12;

	_render_component->_Anim.ColIndex = Idx;
}

uint32_t shield::get_layer_id() const&
{
	return layer_type::EObject;
};

void shield::late_initialize(Transform _Transform)
{
	if (!_transform)return;

	*_transform = std::move(_Transform);

	object_mgr& obj_mgr = object_mgr::instance();

	vec cp = obj_mgr.camera_pos;

	vec w = _transform->_location + (_transform->_dir * _shield_distance);

	auto Effect_SUMMON = obj_mgr.insert_object<Effect>
		(w.x, w.y,
			L"BOTTOM_HOLE", layer_type::EMapDeco, 1, 0, 5.f, FLT_MAX, 200, 150,
			0.8f, 0.8f);

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EShield, ECircle);

	auto sp_collision = _collision_component.lock();
	if (!sp_collision)return;

	sp_collision->bSlide = true;
	sp_collision->PushForce = 5.f;
	sp_collision->bCollisionTargetPushFromForce = false;
	sp_collision->_size = { 50.f,50.0f };
	PaintSizeX = 100;
	PaintSizeY = 105;
	float Scale = 1.f;

	_render_component = std::make_shared<render_component>();
	_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(L"GAIA_ARMOR");
	_render_component->Default_Src_Paint_Size = vec{ PaintSizeX,PaintSizeY };
	_render_component->Dest_Paint_Size = vec{ PaintSizeX * Scale,PaintSizeY * Scale };
	_render_component->_ColorKey = COLOR::MRGENTA();
	_render_component->_Img_src = RECT{ 0,0,PaintSizeX,PaintSizeY };
	_render_component->_Anim.SetAnimationClip(
		{ 12 }, 99999999.f);

	ObjectTag = object::Tag::player_shield;

	// 쉴드 이니셜 방향에 따라 시작 각도 이미지 인덱스만 초기화때 구해놓는다 .
	CalcIdx();

	duration = 10.f;

};
