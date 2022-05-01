#pragma once

namespace dae
{
	class SoundSystem
	{
	public:

		virtual ~SoundSystem() = default;

		virtual void Update() = 0;
		virtual void RegisterSound(const std::string& filename, int id = -1) = 0;
		virtual void PlaySFX(int id, int volume) = 0;

	private:
	};

	class NULLSoundSystem final : public SoundSystem
	{
	public:

		void Update() override {};
		void RegisterSound(const std::string&, int) override {};
		void PlaySFX(int, int) override {};

	private:
	};

	class LoggingSoundSystem final : public SoundSystem
	{
	public:
		LoggingSoundSystem(SoundSystem* ss) : m_pRealSS(ss) {};
		~LoggingSoundSystem() { delete m_pRealSS; };

		void Update() override {};

		void RegisterSound(const std::string& filename, int id = -1) override
		{
			m_pRealSS->RegisterSound(filename, id);
			std::cout << "Registering " << filename << " with id: " << id << std::endl;
		};

		void PlaySFX(int id, int volume) override
		{
			m_pRealSS->PlaySFX(id, volume);
			std::cout << "Playing " << id << " at volume " << volume << std::endl;
		}

	private:
		SoundSystem* m_pRealSS;
	};
}