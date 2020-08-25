#pragma once
#include "singleton_interface.h"
#include "fmod.hpp"
#include <unordered_map>

class sound_mgr : public singleton_interface<sound_mgr>
{
public:
	using SoundType = std::tuple<FMOD::System*, FMOD::Sound*, FMOD::Channel*>;
	std::unordered_map<std::string, SoundType> Sounds;
	static inline constexpr float DefaultVolume = 1.f;
	FMOD::System* FMOD_System = nullptr;
public:
	bool Play(const std::string& SoundKey, bool IsBgm = false, const float Volume = DefaultVolume);
	bool Load(std::string FullPath);
	bool Init();
	bool Frame(const float DeltaTime);
	bool Release();
	bool IsPlay(const std::string& SoundKey);
private:
	std::string CurrentBgmKey;
	const std::string default_path = "..\\..\\Resources\\Sound\\";
};
