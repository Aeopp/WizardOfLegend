#include "pch.h"
#include "FireDragon.h"
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

	_collision_component = collision_mgr::instance().insert(_ptr, collision_tag::EPlayerAttack, ECircle);

	auto sp_collision = _collision_component.lock();

	if (!sp_collision)return;

	sp_collision->_size = { 25.f,25.0f };

	Duration = 100.f;

	PaintSizeX = 180;
	PaintSizeY = 180;
	Scale = 0.8f;
	// 자기자신의 회전속도임
	_speed = 600.f;
	Updown = 1;
	Angle = 45.f;

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
		ParticleRender.push_back(Fire{ vec{_transform->_location},RowNum,0 });

		
		++RowNum;
		if (RowNum >= 3)
			RowNum = 0;
	}
}

void FireDragon::render(HDC hdc, vec camera_pos, vec size_factor)
{
	if (!_render_component) return;
	_render_component->Dest_Loc = _transform->_location - camera_pos - (_render_component->Dest_Paint_Size * 0.5);

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
	float Degree = math::AngleFromVec(rot_dir);
	uint32_t SpriteMapKey = (uint32_t)Degree / 15;
	{
		// 오른쪽
		if (rot_dir.x > 0)
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
			, ps.x, ps.y
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

	for (auto& Particle : ParticleRender)
	{
		vec v = Particle.v;
		v -= camera_pos;
		// ㅠ렌더하기
		GdiTransparentBlt(hdc
			, v.x, v.y
			, 70, 70
			, sp_FireParticle->Get_MemDC()
			, Particle.col * 70       ,Particle.row * 70
			, 70, 70
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

	helper::TEXTOUT(hdc, 300, 300, L"Current Angle : ", math::AngleFromVec(rot_dir));
	helper::TEXTOUT(hdc, 300, 500, L" Init Angle : ", math::AngleFromVec(_transform->_dir));
}

Event FireDragon::update(float dt)
{
	Event _Event = actor::update(dt);
	if (!_transform)return Event::Die;

	CurrentAngle += (dt * 90.f) * Updown;
	if (CurrentAngle > 45.f)Updown = -1;
	if (CurrentAngle < -45.f)Updown = 1;

	// CurrentAngle = math::LOOP({-45.f,+45.f}, CurrentAngle);
	rot_dir = math::rotation_dir_to_add_angle(rotation_center_dir, CurrentAngle);


	rotation_center += _transform->_dir * _speed * dt;
	_transform->_location = rotation_center + (rot_dir * 100.f);


	{
		ParticleRender[CurrentParticleIndex].v = _transform->_location;

		++CurrentParticleIndex;
		if (CurrentParticleIndex >= ParticleNum)
		{
			CurrentParticleIndex = 0;
		}
	}
	

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

	bDie = true;
}
