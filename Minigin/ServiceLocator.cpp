#include "NightOwlEnginePCH.h"
#include "ServiceLocator.h"

dae::NULLSoundSystem dae::ServiceLocator::m_SSNULL{};
dae::SoundSystem* dae::ServiceLocator::m_pSSInstance = &m_SSNULL;