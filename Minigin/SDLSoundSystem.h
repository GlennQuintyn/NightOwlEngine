#pragma once
#include "SoundSystem.h"

namespace dae
{
	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem(const std::string_view soundDataPath);
		~SDLSoundSystem();

		void Update() override {};//the update is implemented is on a seperate thread

		void RegisterSound(const std::string& filename, int id = -1) override;
		void PlaySFX(int id, int volume) override;

		SDLSoundSystem(const SDLSoundSystem& other) = delete;
		SDLSoundSystem(SDLSoundSystem&& other) = delete;
		SDLSoundSystem& operator=(const SDLSoundSystem& other) = delete;
		SDLSoundSystem& operator=(SDLSoundSystem&& other) = delete;
	private:
		class SDLSoundSystemImpl;
		SDLSoundSystemImpl* m_pImpl;
	};
}
