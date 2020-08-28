#include "pch.h"
#include "sound_mgr.h"
#include <fstream>
#include <filesystem>
#include <iostream>

void sound_mgr::initialize()
{
	Init();

	namespace fs =   std::filesystem;

	for (const fs::directory_entry& entry :
		fs::directory_iterator(fs::current_path()/".."/".."/"Resources"/"Sound")) {
		bool bLoad =Load(entry.path().string(),entry.path().stem().string());
	}
}

bool sound_mgr::Play( std::string SoundKey,
	bool IsBgm, const float Volume) {

	namespace fs = std::filesystem;

	if (Sounds.empty() || (IsBgm == true && CurrentBgmKey == SoundKey)) {
		return false;
	}

	FMOD_RESULT HR;
	bool isplay = false;

	if (auto iter = Sounds.find(SoundKey);
		iter != std::end(Sounds)) {
		auto& [System, Sound, Channel] = iter->second;
		{
			Channel->isPlaying(&isplay);

			if (isplay == true && IsBgm == true) {
				Channel->stop();
			};
			HR = System->playSound(Sound, nullptr, false,
				&Channel);
			Channel->setVolume(Volume == DefaultVolume ? DefaultVolume : Volume);

			if (HR != FMOD_OK) {
				return  false;
			}
			else if (IsBgm == true) {
				if (auto iter = Sounds.find(CurrentBgmKey);
					iter != std::end(Sounds)) {
					auto& [System, Sound, Channel] = iter->second;
					Channel->stop();
				}
				CurrentBgmKey = SoundKey;
			}
			return  true;
		}
	}
	else {

	}
	return false;
};

bool sound_mgr::Load(std::string FullPath, std::string Key) {

	if (FMOD_System == nullptr) {
		return false;
	};
	// ���� ���� �����ؼ� ����
	if (auto iter = Sounds.find(Key);
		iter == std::end(Sounds) == true) {
		SoundType InsertSound;
		auto& [System, Sound, Channel] = InsertSound;
		System = FMOD_System;

		FMOD_RESULT HR = System->createSound(FullPath.c_str(), FMOD_DEFAULT, 0, &Sound);

		if (HR != FMOD_OK) {
			return false;
		};

		Channel->setVolume(DefaultVolume);
		Sounds.insert(iter, { std::move(Key)  , InsertSound });
	}
	// �̹� ���尡 ������
	else {

	}
	return true;
};

bool sound_mgr::Frame(const float DeltaTime) {
	if (FMOD_System == nullptr) return false;
	// update �Լ��� �������Ӹ��� �ݵ�� ȣ����������� �䱸��
	FMOD_System->update();
	return true;
}
bool sound_mgr::Release()
{
	for (auto& [key, SoundObject] : Sounds) {
		auto& [System, Sound, Channel] = SoundObject;
		Sound->release();
	}
	Sounds.clear();
	return true;
}
bool sound_mgr::IsPlay(const std::string& SoundKey)
{
	bool bPlay{};

	auto SoundIter = Sounds.find(SoundKey);

	if (std::end(Sounds) != SoundIter) {
		auto& [Key, SoundObject] = *SoundIter;
		auto& [System, Sound, Channel] = SoundObject;
		Channel->isPlaying(&bPlay);
	};

	return bPlay;
}
bool sound_mgr::Init()
{
	FMOD_RESULT F_Result;

	F_Result = FMOD::System_Create(&FMOD_System);

	{
		// FMOD �ý��� �ε� ����
		if (F_Result != FMOD_OK)
			return false;
		F_Result = FMOD_System->init(32, FMOD_INIT_NORMAL, 0);
		if (F_Result != FMOD_OK)
			return false;
	}

	return true;
};
