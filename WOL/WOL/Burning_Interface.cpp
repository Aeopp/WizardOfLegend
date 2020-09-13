#include "pch.h"
#include "Burning_Interface.h"

#include "Bmp_mgr.h"
#include "object_mgr.h"
#include "Bmp.h"


void Burning_Interface::burning_update(float dt)
{
	_Burning_Info.update(dt);
	
};

void Burning_Interface::Burning_render(HDC hdc, vec location)
{
	_Burning_Info.render(hdc,location);
}

bool Burning_Interface::probability_calculation(float _Percentage)
{
	float DICE = math::Rand<float>({ 0.01f,1.f });
	return !(DICE >= _Percentage);
}

Burning_Interface::Burning::Particle_Info Burning_Interface::Burning::Particle_Info::MakeInfo(vec particle_range ,  vec render_world_size_range)
{
	Particle_Info _Info;
	_Info.ColIdx = 0;
	_Info.RowIdx = math::Rand<int>({0, 3});
	_Info.render_location = vec::RandVecFromRange(particle_range);
	_Info.render_dir = math::RandVec() * 5;
	int RandParticleSize = math::Rand<int>({ render_world_size_range . x , render_world_size_range . y });
	
	_Info.render_world_size = vec{ RandParticleSize  , RandParticleSize };
	
	return _Info;
}

std::shared_ptr<Bmp> Burning_Interface::Burning::GetImg()
{
	static auto sp_Img = Bmp_mgr::instance().Find_Image_SP(L"FIRE_PARTICLE");
	return sp_Img;
}

void Burning_Interface::Burning::burn(vec msgLocation)
{
	if (RemainTime > 0)return;

	object_mgr::instance().TextEffectMap[Burning_Interface::EffectColor].
		push_back({ 
	msgLocation,vec{0,1.f},2.5f,30,L"Burning!!" });

	RemainTime = Duration;
	
	_Particle_Infos.reserve(ParticleNum);

	std::generate_n(std::back_inserter(_Particle_Infos),
		ParticleNum, [this]()
		{return Particle_Info::MakeInfo(particle_range  , render_world_size_range );});
}

bool Burning_Interface::Burning::IsBurning() const &
{
	return RemainTime > 0;
}

bool Burning_Interface::Burning::update(float dt)
{
	RemainTime -= dt;
	if (RemainTime < 0)return false;

	for(auto & _Particle_Info : _Particle_Infos)
	{
		_Particle_Info.render_location += _Particle_Info.render_dir;
		
		_Particle_Info.AnimTick -= dt;
		if (_Particle_Info.AnimTick < 0)
		{
			_Particle_Info.AnimTick = Burning::AnimDelta;

			++_Particle_Info.ColIdx;
			if (_Particle_Info.ColIdx > 4)
			{
				_Particle_Info = Particle_Info::MakeInfo(particle_range,render_world_size_range);
			}
		}
	}
	
	return true;
}

void Burning_Interface::Burning::render(HDC hdc, vec location)
{
	if (!IsBurning())return;

	auto sp_Img = GetImg();
	if (!sp_Img)return;

	for (auto& _Particle_Info : _Particle_Infos)
	{
		vec DestLoc =  ( location  +_Particle_Info.render_location ) - (PaintSize * 0.5f);

		GdiTransparentBlt(hdc, DestLoc.x, DestLoc.y,
			_Particle_Info.render_world_size.x, _Particle_Info.render_world_size.y,
			sp_Img->Get_MemDC(), PaintSize.x * _Particle_Info.ColIdx,
			_Particle_Info.RowIdx * PaintSize.y,
			PaintSize.x, PaintSize.y, COLOR::MRGENTA());
	}
}
