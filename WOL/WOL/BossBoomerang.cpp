#include "pch.h"
#include "BossBoomerang.h"
#include "collision_mgr.h"
#include "render_component.h"
#include "Color.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "sound_mgr.h"




void BossBoomerang::initialize()
{
	object::initialize();
	RAND_SOUNDPLAY("FIRE_DRAGON", { 1,3 }, 1.f, false);

	_collision_component = collision_mgr::instance().
		insert(_ptr, collision_tag::EMonsterAttack, ECircle);

	auto sp_collision = _collision_component.lock();

	if (!sp_collision)return;


	Duration = 2;

	ScaleX = 0.8f;
	ScaleY = 0.8f;

	PaintSizeX = 200;
	PaintSizeY = 200;
	// 직선 운동 속도
	
	AnimSpeed = 2.f;
	sp_collision->bSlide = true;
	sp_collision->bCollisionTargetPushFromForce = true;
	sp_collision->PushForce = 15.f;
	sp_collision->_size = { 60.f,60.f };
	sp_collision->bCollision = true;
	sp_collision->bRender = true;
	sp_collision->bTileHitEffect = false;

	bAttacking = true;
	ObjectTag = object::Tag::monster_attack;

	bAttacking = true;

	AnimRowIdx = 0;
	CurrentColIdx = 0;
	CurrentAnimRemainTime = AnimDelta;

	wp_Image = Bmp_mgr::instance().Find_Image_WP(L"SWORDMAN_ATTACK");
	wp_ResetImg = Bmp_mgr::instance().Find_Image_WP(L"SWORDMAN_ATTACK_RESET");
	wp_RotImg = Bmp_mgr::instance().Find_Image_WP(L"SWORDMAN_ATTACK_ROTATION");

	Attack = { 30,40 };

	UniqueID = EObjUniqueID::Boomerang;

	RotationSpeed = 900.f;

	HomingRotationCurrentSpeed = 0.5f;
	HomingRotationacceleration = 0.1f;
	_speed = 900.f;
}

Event BossBoomerang::update(float dt)
{
	Event _event = object::update(dt);

	HitTileEffectRemainTime -= dt;
	
	Duration -= dt;
	if (Duration < 0)return Event::Die;

	if (!_transform)return Event::Die;
	auto sp_Target = wp_Target.lock();
	if (!sp_Target) return Event::Die;
	vec CurrentDir = _transform->_dir;
	vec MyLocation = _transform->_location;
	vec TargetDis = sp_Target->_transform->_location - MyLocation;
	vec TargetDir = TargetDis.get_normalize();

	float Z = CurrentDir.CrossZ(TargetDir);
	float Dot = CurrentDir.dot(TargetDir);

	float AngleDegree = math::radian_to_degree(std::acosf(Dot));

	if (Z > 0)
		AngleDegree *= -1;

	_transform->_dir =
		math::rotation_dir_to_add_angle(CurrentDir, AngleDegree * dt *
			HomingRotationCurrentSpeed);

	HomingRotationCurrentSpeed += HomingRotationacceleration * dt;

	_transform->_location += _transform->_dir * dt * _speed;

	ImgAngle += dt * RotationSpeed;

	return _event;
}

void BossBoomerang::render(HDC hdc, vec camera_pos, vec size_factor)
{
	CalcImgAngle(ImgAngle);

	HDC hMemDC = wp_Image.lock()->Get_MemDC();
	HDC hPlgDC = wp_RotImg.lock()->Get_MemDC();
	HDC hResetDC = wp_ResetImg.lock()->Get_MemDC();

	PlgBlt(hPlgDC, RotPts.data(), hMemDC, 0, 0, PaintSizeX, PaintSizeY, NULL, NULL, NULL);

	vec v = _transform->_location - camera_pos -
		(vec{ PaintSizeX,PaintSizeY } *0.5);

	GdiTransparentBlt(hdc,
		v.x, v.y
		, PaintSizeX * ScaleX, PaintSizeY * ScaleY
		, hPlgDC
		, 0, 0
		, PaintSizeX, PaintSizeY
		, RGB(255, 0, 255));

	BitBlt(hPlgDC, 0, 0, PaintSizeX, PaintSizeY, hResetDC, 0, 0, SRCCOPY);
}

uint32_t BossBoomerang::get_layer_id() const&
{
	return layer_type::EEffect;
}

void BossBoomerang::Hit(std::weak_ptr<object> _target)
{
	object::Hit(_target);
}

void BossBoomerang::HitTile(RECT TileRt)
{
	object::HitTile(TileRt);

	vec dir = _transform->_dir;
	dir *= -1;

	if (HitTileEffectRemainTime < 0)
	{
		HitTileEffectRemainTime = Boomerang::HitTileEffectTick;
		collision_mgr::instance().HitEffectPush(_transform->_location, 0.3f);
		RAND_SOUNDPLAY("WALL_HITTED_FIREDRAGON", { 0,2 }, 1.f, false);
	}

	int RandAngle = math::Rand<int>({ -1,+1 });

	dir = math::rotation_dir_to_add_angle(dir, RandAngle * 45);

	_transform->_dir = dir;

	CalcImgAngle(math::radian_to_degree(atan2f(dir.y, dir.x)));
}


void BossBoomerang::SetUp(std::weak_ptr<class object> wp_AttackTarget,vec InitLocation, vec InitDir)
{
	if (!_transform)return;
	_transform->_location = std::move(InitLocation);
	_transform->_dir = std::move(InitDir);
	wp_Target = std::move( wp_AttackTarget);
	CalcImgAngle(math::radian_to_degree(atan2f(InitDir.y, InitDir.x)));
}

void BossBoomerang::CalcImgAngle(float RotationImgDegree)
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

BossBoomerang::~BossBoomerang() noexcept
{
	RAND_SOUNDPLAY("FIRE_DRAGON_DIE", { 0,3 }, 1.f, false);
}
