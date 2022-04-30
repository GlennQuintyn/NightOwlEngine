#pragma once

namespace dae
{
	class SoundSystem
	{
	public:

		virtual ~SoundSystem() = default;
		virtual void PlaySFX() = 0;
		virtual void RegisterSound() = 0;

	private:
	};

	class NULLSoundSystem final : public SoundSystem
	{
	public:

		void PlaySFX() override {};
		void RegisterSound() override {};

	private:
	};

	class LoggingSoundSystem final : public SoundSystem
	{
	public:
		LoggingSoundSystem(SoundSystem* ss) : m_pRealSS(ss) {};
		~LoggingSoundSystem() { delete m_pRealSS; };

		void RegisterSound(/*const sound_id id, const std::string_view path*/) override
		{
			//m_pRealSS->registerSound(/*id I pa h*/);
		};

		void PlaySFX(/*const sound_id id, const float volu e*/) override
		{
			//_real_ss->play(id, volume);
			//std::cout << "Playing " << id << " at volume " << volume << std::endl;
		}

	private:
		SoundSystem* m_pRealSS;
	};
}