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
	void initialize();

	// Ȯ���ھ��� �����̸��� �Է����ּ���.
	bool Play(std::string SoundKey, bool IsBgm = false, const float Volume = DefaultVolume);
	bool Load(std::string FullPath,std::string Key);
	bool Init();
	bool Frame(const float DeltaTime);
	bool Release();
	bool IsPlay(const std::string& SoundKey);

	void RandSoundKeyPlay(const std::string& SoundKey,
		std::pair<uint8_t, uint8_t> Range, float Volume);
private:
	std::string CurrentBgmKey;
};
