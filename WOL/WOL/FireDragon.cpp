#include "pch.h"
#include "FireDragon.h"
#include "sound_mgr.h"
#include "collision_mgr.h"
#include "render_component.h"
#include "Color.h"
#include "Bmp_mgr.h"
#include "helper.h"
#include "game.h"
#include "Bmp.h"



void FireDragon::initialize()
{
	actor::initialize();

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EFireDragon, ECircle);

	auto sp_collision = _collision_component.lock();

	if (!sp_collision)return;

	sp_collision->_size = { 25.f,25.0f };

	Duration = 100.f;

	PaintSizeX = 180;
	PaintSizeY = 180;
	Scale = 0.7f;
	// 자기자신의 회전속도임
	_speed = 200.f;
	Updown = 1;
	amplitudeSpeed = 1.f; 
	UpdownDistance = 70.f;

	particlePaintSize = { 70,70 };
	ParticleBetWeen = 70.f;
	ParticlePaintScale = 0.7f;

	_render_component = std::make_shared<render_component>();
	_render_component->wp_Image = Bmp_mgr::instance().Find_Image_WP(L"SKILL_FIREDRAGON_COM");
	_render_component->Default_Src_Paint_Size = vec{ PaintSizeX,PaintSizeY };
	_render_component->Dest_Paint_Size = vec{ PaintSizeX * Scale,PaintSizeY * Scale };
	_render_component->_ColorKey = RGB(255, 0, 255);
	_render_component->_Img_src = RECT{ 0,0,PaintSizeX,PaintSizeY };
	_render_component->_Anim.SetAnimationClip(
		{ 19 }, 1.f);

	factor = 1.f;
	_Shadow.bShadow = false;

	FireParticle= Bmp_mgr::instance().Find_Image_WP(L"FIRE_PARTICLE");
	
	int RowNum = 0;
	for (int i = 0; i < ParticleNum; ++i)
	{
		ParticleRender.push_back(Fire{ RowNum,0 });
		
		++RowNum;
		if (RowNum >= 3)
			RowNum = 0;
	}	
	
	UniqueID = EObjUniqueID::FIREDRAGON;

	ObjectTag = object::Tag::player_attack;
	bAttacking = true;
	LaunchSoundPlay();

}

void FireDragon::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!_render_component) return;
	_render_component->Dest_Loc = _transform->_location - camera_pos - (_render_component->Dest_Paint_Size * 0.5 * Scale);

	_Shadow.render(hdc, camera_pos);

	auto sp_Image = _render_component->wp_Image.lock();
	if (!sp_Image)return;

	const vec& dl = _render_component->Dest_Loc;
	const vec& ps = _render_component->Dest_Paint_Size;

	RECT CullingRect{ dl.x,dl.y,ps.x + dl.x,ps.y + dl.y };
	RECT _rt = game::instance().client_rect;

	HDC _BDC = sp_Image->Get_MemDC();

	const RECT& s = _render_component->_Img_src;
	const vec& ds = _render_component->Default_Src_Paint_Size;


	uint32_t AnimColIndex{}, AnimRowIndex{};
	float Degree = CurrentAngle;
	uint32_t SpriteMapKey = (uint32_t)Degree / 15;
	{
		// 오른쪽
		if (Degree <  90.f  || Degree > 270.f)
		{
			AnimRowIndex = 0;
			AnimColIndex = RightDirSpriteTable[SpriteMapKey];
		}
		// 왼쪽
		else
		{
			AnimRowIndex = 1;
			AnimColIndex = LeftDirSpriteTable[SpriteMapKey];
		}
	}
		
	
	switch (_render_component->_RenderDesc)
	{
	case Transparent:
		GdiTransparentBlt(hdc
			, dl.x, dl.y
			, ps.x * Scale, ps.y*Scale
			, _BDC
			, s.left + AnimColIndex * ds.x, s.top + AnimRowIndex * ds.y
			, s.right, s.bottom
			, _render_component->_ColorKey);
		break;
	default:
		break;
	}

	auto sp_FireParticle = FireParticle.lock();
	if (!sp_FireParticle)return;

	for(int i=1;i<=ParticleNum;++i)
	{
		vec v = rotation_center;
		vec w = particlePaintSize;

		v -=( _transform->_dir * ParticleBetWeen*i);
		v += Cross * amplitude * (Tick - (1.f/(float)ParticleNum)*i);
		v -= camera_pos;
		v -= (particlePaintSize * 0.5 * ParticlePaintScale);

		Fire& Particle = ParticleRender[i - 1]; 

		GdiTransparentBlt(hdc
			, v.x, v.y
			, w.x * ParticlePaintScale , w.x * ParticlePaintScale 
			, sp_FireParticle->Get_MemDC()
			, Particle.col * w.x       ,Particle.row * w.y
			, w.x, w.y
			, _render_component->_ColorKey);

		ParticleDelta -= DeltaTime;

		if (ParticleDelta < 0)
		{
			ParticleDelta = DefaultParticleDelta;
			++Particle.col;
			if (Particle.col >= 5) 
			{
				Particle.col = 0;
			}
		}
	};

	if (bDebug)
	{
		helper::TEXTOUT(hdc, 300, 300, L"Current Angle : ", CurrentAngle);
		helper::TEXTOUT(hdc, 300, 500, L" Init Angle : ", math::AngleFromVec(_transform->_dir));
	}
}

Event FireDragon::update(float dt)
{
	// 지속시간다되면 
	// DieSoundPlay();
	Event _Event = actor::update(dt);
	if (!_transform)return Event::Die;

	Tick += (dt*amplitudeSpeed)*Updown;
	if (Tick >= 1.f)Updown = -1.f;
	if (Tick <= -1.f)Updown = +1.f;

    // 직선의 중심점은 일단 이동
	rotation_center += _transform->_dir * _speed * dt;
	 // 직선의 중심점에서 진폭만큼 더해주기
	_transform->_location = rotation_center + (Cross * amplitude * Tick);
	
	CurrentAngle = math::AngleFromVec(_transform->_dir) + (45.f* Tick);

	return _Event;
}

uint32_t FireDragon::get_layer_id() const&
{
	return layer_type::EEffect;
}

void FireDragon::Hit(std::weak_ptr<class object> _target)
{
	object::Hit(_target);
}

void FireDragon::HitTile(RECT rt)
{
	object::HitTile(rt);
	WallHitSoundPlay();
	bDie = true;
}

void FireDragon::LaunchSoundPlay()
{
	RAND_SOUNDPLAY("FIRE_DRAGON", { 1,3 }, 1.f, false);

}

void FireDragon::DieSoundPlay()
{
	RAND_SOUNDPLAY("FIRE_DRAGON_DIE", { 0,3 }, 1.f, false);

}

void FireDragon::WallHitSoundPlay()
{
	RAND_SOUNDPLAY("WALL_HITTED_FIREDRAGON", { 0,3 }, 1.f, false);

}
