#pragma once
#include "SoundSystem.h"

namespace dae
{
	class ServiceLocator
	{
	public:
		static SoundSystem& GetSS() { return *m_pSSInstance; };

		//The service locator owns the sound system for easier cleanup
		static void RegisterSoundSystem(SoundSystem* ss)
		{
			//check if its not the null type, if not then delete it
			if (m_pSSInstance != &m_SSNULL)
				delete m_pSSInstance;

			m_pSSInstance = ss == nullptr ? &m_SSNULL : ss;
		}

		ServiceLocator() = delete;
		~ServiceLocator() = delete;
		ServiceLocator(const ServiceLocator& other) = delete;
		ServiceLocator(ServiceLocator&& other) = delete;
		ServiceLocator& operator=(const ServiceLocator& other) = delete;
		ServiceLocator& operator=(ServiceLocator&& other) = delete;

	private:
		static SoundSystem* m_pSSInstance;
		static NULLSoundSystem m_SSNULL;
	};
}

