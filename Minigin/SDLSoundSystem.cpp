#include "NightOwlEnginePCH.h"
#include "SDLSoundSystem.h"
#include "SDL_mixer.h"
#include "ResourceManager.h"
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace dae
{
	class SDLSoundSystem::SDLSoundSystemImpl
	{
	public:

		SDLSoundSystemImpl(const std::string_view soundDataPath)
			: m_Head{}
			, m_Tail{}
			, m_RingBuffer{}
			, m_SoundThread{}
			, m_CV{}
			, m_RunThread{ true }
		{
			if (soundDataPath.empty())
			{
				Logger::GetInstance().LogError("ResourceManager:\tNO DATA PATH WAS GIVEN!");
				return;
			}

			m_SoundPath = soundDataPath;

			//
			//Base init of SDL MIXER
			//
			SDL_version compile_version{};
			const SDL_version* link_version = Mix_Linked_Version();
			SDL_MIXER_VERSION(&compile_version);

			link_version;
			printf("compiled with SDL_mixer version: %d.%d.%d\n",
				compile_version.major, compile_version.minor, compile_version.patch);
			printf("running with SDL_mixer version: %d.%d.%d\n",
				link_version->major, link_version->minor, link_version->patch);


			//will need standard wav and maybe also mp3 file support
			//checks if mp3 libraries were able to load
			if (Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3)
			{
				throw std::runtime_error("Mix_Init: Failed to init mp3 support!\n Mixer error: " + std::string(Mix_GetError()));
			}

			//
			//Setup of SDL MIXER
			//
			// Set up the audio stream
			int result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
			if (result < 0)
				throw std::runtime_error(std::string("Unable to open audio: ") + SDL_GetError());


			result = Mix_AllocateChannels(8);
			if (result < 0)
				throw std::runtime_error(std::string("Unable to allocate mixing channels: ") + SDL_GetError());

			//make an infinte loop that is not quite infinite
			m_SoundThread = std::jthread([this]
				{
					while (m_RunThread)
					{
						std::unique_lock<std::mutex> lk(m_Mutex);
						//If there are no pending requests, do nothing.
						m_CV.wait(lk, [this] {  return (m_Head != m_Tail) || !m_RunThread; });
						//m_Mutex.unlock();
						lk.unlock();

						Update();
					}
				});
		};

		~SDLSoundSystemImpl()
		{
			m_RunThread = false;
			//notify to wakeup the condition variable linked to the seperate thread
			m_CV.notify_all();
			//join thread to be sure that it stops the loop and won't try and update when it should close down
			m_SoundThread.join();

			int frequency, channels;
			Uint16 format;
			//you have to call Mix_CloseAudio as many times as you opened it, so for safety i get the ammount that is open
			//and close them all in a small loop as its just closing time which doesn't matter that much as runtime
			int count = Mix_QuerySpec(&frequency, &format, &channels);
			for (int index = 0; index < count; index++)
			{
				Mix_CloseAudio();
			}
			Mix_Quit();
		}

		//actually plays the head from the ring buffer
		void Update()
		{
			SoundInfo firstInQueue{};
			{
				//bool yesu = m_Mutex.try_lock();
				//yesu;
				std::scoped_lock mylk(m_Mutex);
				firstInQueue = m_RingBuffer[m_Head];

				m_Head = (m_Head + 1) % MAX_PENDING;
			}

			Mix_Chunk* pSoundChunk = m_MapOfSounds.at(firstInQueue.id);
			Mix_VolumeChunk(pSoundChunk, firstInQueue.volume);
			Mix_PlayChannel(-1, pSoundChunk, 0);
		}

		//add sound to soundmap
		void RegisterSound(const std::string& filename, int id)
		{
			//check if sound was already registered
			auto it = m_MapOfSounds.find(id);
			if (it != m_MapOfSounds.end())
			{
				//std::string warning{ "SDLSOUNDSYSTEM: SOUND ALREADY EXISTS WITH ID: " + std::to_string(id) };
				Logger::GetInstance().LogWarning("SDLSOUNDSYSTEM: SOUND ALREADY EXISTS WITH ID: " + std::to_string(id));
				return;
			}

			//if new, then load chunk
			Mix_Chunk* pSoundChunk;
			const std::string fileLocation{ m_SoundPath + filename };
			//const std::string fileLocation{ m_SoundPath + "test.wav" };
			pSoundChunk = Mix_LoadWAV(fileLocation.c_str());
			if (!pSoundChunk)
			{
				Logger::GetInstance().LogWarning("SDLSOUNDSYSTEM: AUDIO FILE WAS NOT FOUND: " + filename);
				return;
			}

			id = (id <= -1) ? int(m_MapOfSounds.size()) : id;
			m_MapOfSounds.emplace(id, pSoundChunk);
		};

		//add desired sound to ringbuffer
		void PlaySFX(int id, int volume = MIX_MAX_VOLUME / 2)
		{
			//check if sound exists already
			auto it = m_MapOfSounds.find(id);
			if (it == m_MapOfSounds.end())
				return;

			// Add to the end of the list.
			{
				std::scoped_lock mylk(m_Mutex);
				// Walk the pending requests to find duplicates, if so change volume to more recent requested.
				for (int i = m_Head; i != m_Tail; i = (i + 1) % MAX_PENDING)
				{
					if (m_RingBuffer[i].id == id)
					{
						m_RingBuffer[i].volume = volume;
						return;
					}
				}
				//check so that it doesn't overwrite sounds that should still be played
				assert((m_Tail + 1) % MAX_PENDING != m_Head);

				// Add to the end of the list.
				//std::scoped_lock mylk(m_Mutex);
				m_RingBuffer[m_Tail].id = id;
				m_RingBuffer[m_Tail].volume = volume;
				m_Tail = (m_Tail + 1) % MAX_PENDING;
			}

			m_CV.notify_all();
		};

		// Mix_Pause(-1) and Mix_PauseMusic instead, to pause.
			//Use Mix_Resume(-1) and Mix_ResumeMusic instead, to unpause.

	private:
		struct SoundInfo
		{
			int id;
			int volume;
		};

		//sound event queue logic
		static const int MAX_PENDING = 32;
		SoundInfo m_RingBuffer[MAX_PENDING];
		int m_Head;
		int m_Tail;

		std::jthread m_SoundThread;
		std::condition_variable m_CV;
		std::mutex m_Mutex;
		bool m_RunThread;

		std::string m_SoundPath;
		std::map<int, Mix_Chunk*> m_MapOfSounds;
	};
}

using namespace dae;

SDLSoundSystem::SDLSoundSystem(const std::string_view soundDataPath)
{
	m_pImpl = new SDLSoundSystem::SDLSoundSystemImpl{ soundDataPath };
}

SDLSoundSystem::~SDLSoundSystem()
{
	delete m_pImpl;
}

//void SDLSoundSystem::Update()
//{
//	//m_pImpl->Update();
//}

void SDLSoundSystem::RegisterSound(const std::string& filename, int id)
{
	m_pImpl->RegisterSound(filename, id);
}

void SDLSoundSystem::PlaySFX(int id, int volume)
{
	m_pImpl->PlaySFX(id, volume);
}