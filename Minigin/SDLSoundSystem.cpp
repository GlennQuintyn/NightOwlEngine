#include "NightOwlEnginePCH.h"
#include "SDLSoundSystem.h"

namespace dae
{
	class SDLSoundSystem::SDLSoundSystemImpl
	{
	public:

	private:

	};
}

using namespace dae;

SDLSoundSystem::SDLSoundSystem()
{
	m_pImpl = new SDLSoundSystem::SDLSoundSystemImpl();
}

SDLSoundSystem::~SDLSoundSystem()
{
	delete m_pImpl;
}