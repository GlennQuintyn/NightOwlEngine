#pragma once
#include "SoundSystem.h"

namespace dae
{
	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem();

		void PlaySFX() override {};
		void RegisterSound(/*const sound_id id, const std::string_view path*/) override {};

	private:
		class SDLSoundSystemImpl;
		SDLSoundSystemImpl* m_pImpl;
	};
}
