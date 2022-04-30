#pragma once

#include "SoundSystem.h"

namespace dae
{
	//class SoundSystem;
	//class NULLSoundSystem;

	class ServiceLocator
	{
	public:
		~ServiceLocator() { delete m_pSSInstance; }
		
		static SoundSystem& GetSS() { return *m_pSSInstance; };

		//The service locator owns the sound system for easier cleanup
		static void RegisterSoundSystem(SoundSystem* ss)
		{
			m_pSSInstance = ss == nullptr ? &m_SSNULL : ss;
		}

	private:
		static SoundSystem* m_pSSInstance;
		static NULLSoundSystem m_SSNULL;
	};

	//NULLSoundSystem ServiceLocator::m_SSNULL{};
	//SoundSystem* ServiceLocator::m_pSSInstance = &m_SSNULL;
}

