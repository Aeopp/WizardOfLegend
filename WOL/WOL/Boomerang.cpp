#include "pch.h"
#include "Boomerang.h"
#include "collision_mgr.h"
#include "render_component.h"
#include "Color.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "sound_mgr.h"


void Boomerang::initialize()
{
	actor::initialize();
	RAND_SOUNDPLAY("FIRE_DRAGON", { 1,3 }, 1.f, false);

	_collision_component = collision_mgr::instance().
	insert(_ptr, collision_tag::EPlayerAttack, ECircle);

	auto sp_collision = _collision_component.lock();

	if (!sp_collision)return;


	Duration = 10.f;

	ScaleX = 0.5f;
	ScaleY = 0.5f;

	PaintSizeX = 200;
	PaintSizeY = 200;
	// 직선 운동 속도
	_speed = 1500.f;
	AnimSpeed = 2.f;
	sp_collision->bSlide = true;
	sp_collision->bCollisionTargetPushFromForce = true;
	sp_collision->PushForce = 30.f;
	sp_collision->_size = { 60.f,60.f };
	sp_collision->bCollision = true;
	sp_collision->bRender = true;
	sp_collision->bTileHitEffect = true;

	bAttacking = true;
	ObjectTag = object::Tag::player_attack;

	bAttacking = true;
	_Shadow.correction = { 0,60 };
	_Shadow.bShadow = false;
	AnimRowIdx = 0;
	CurrentColIdx = 0;
	CurrentAnimRemainTime = AnimDelta;
	
	_render_component = std::make_shared<render_component>();
	_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(L"SWORDMAN_ATTACK");
	_render_component->Default_Src_Paint_Size = vec{ PaintSizeX,PaintSizeY };
	_render_component->Dest_Paint_Size = vec{ PaintSizeX * ScaleX,PaintSizeY * ScaleY };
	_render_component->_ColorKey = RGB(255, 0, 255);
	_render_component->_Img_src = RECT{ 0,0,PaintSizeX,PaintSizeY };
	_render_component->_Anim.SetAnimationClip(
		{ 4,4,4,4,4,4 }, AnimSpeed);

	wp_ResetImg = Bmp_mgr::instance().Find_Image_WP(L"SWORDMAN_ATTACK_RESET");
	wp_RotImg = Bmp_mgr::instance().Find_Image_WP(L"SWORDMAN_ATTACK_ROTATION");

	Attack = { 30,40};

	UniqueID = EObjUniqueID::Boomerang;

}

Event Boomerang::update(float dt)
{
	Event _event = actor::update(dt);

	Duration -= dt;
	if (Duration < 0)return Event::Die;

	_transform->_location +=_transform->_dir * _speed * dt;
	

	return _event;
}

void Boomerang::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!_render_component)return;

	//CalcImgAngle(ImgAngle);

	HDC hMemDC = _render_component->wp_Image.lock()->Get_MemDC();
	HDC hPlgDC = wp_RotImg.lock()->Get_MemDC();
	HDC hResetDC = wp_ResetImg.lock()->Get_MemDC();

	PlgBlt(hPlgDC, RotPts.data(), hMemDC, 0, 0, PaintSizeX, PaintSizeY, NULL, NULL, NULL);

	vec v = _transform->_location - camera_pos - (_render_component->Dest_Paint_Size * 0.5);

	GdiTransparentBlt(hdc,
		 v.x, v.y
		, PaintSizeX*ScaleX, PaintSizeY*ScaleY
		, hPlgDC
		, 0, 0
		, PaintSizeX, PaintSizeY
	, RGB(255, 0, 255));

	BitBlt(hPlgDC, 0, 0, PaintSizeX, PaintSizeY, hResetDC, 0, 0, SRCCOPY);
}

uint32_t Boomerang::get_layer_id() const&
{
	return layer_type::EEffect;
}

void Boomerang::Hit(std::weak_ptr<object> _target)
{
	actor::Hit(_target);

}

void Boomerang::HitTile(RECT TileRt)
{
	actor::HitTile(TileRt);

 	vec dir = _transform->_dir;
	dir *= -1;

	RAND_SOUNDPLAY("WALL_HITTED_FIREDRAGON", { 0,2 }, 1.f, false);
	
	float RandAngle = math::Rand<int>({ -45,+45 });

	dir =math::rotation_dir_to_add_angle(dir,RandAngle);

	_transform->_dir = dir;

	CalcImgAngle(math::radian_to_degree(atan2f(dir.y, dir.x)));

}


void Boomerang::CalcImgAngle(float RotationImgDegree)
{
	float x = PaintSizeX / 2;
	float y = PaintSizeY / 2;
	float Dis = vec{ x   , y }.length();
	
	RotPts[0].x = (LONG)(x + cosf(
		math::degree_to_radian(135.f + RotationImgDegree)) * Dis);

	RotPts[0].y = (LONG)(y + sinf(
		math::degree_to_radian(135.f + RotationImgDegree)) * Dis);

	RotPts[1].x = (LONG)(x + cosf(
		math::degree_to_radian(45.f + RotationImgDegree)) * Dis);

	RotPts[1].y = (LONG)(y + sinf(
		math::degree_to_radian(45.f + RotationImgDegree)) * Dis);

	RotPts[2].x = (LONG)(x + cosf(
		math::degree_to_radian(225.f + RotationImgDegree)) * Dis);

	RotPts[2].y = (LONG)(y + sinf(
		math::degree_to_radian(225.f + RotationImgDegree)) * Dis);
}

Boomerang::~Boomerang() noexcept
{
	RAND_SOUNDPLAY("FIRE_DRAGON_DIE", { 0,3 }, 1.f, false);

}
