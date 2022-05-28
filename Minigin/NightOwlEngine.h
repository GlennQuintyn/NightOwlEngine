#pragma once
struct SDL_Window;

namespace dae
{
	class GameObject;

	class NightOwlEngine
	{
	public:

		NightOwlEngine(int windowWidth, int windowHeight);
		~NightOwlEngine() = default;

		void Initialize();
		//void LoadGame() /*const*/;
		void Run();
		void Cleanup();

		SDL_Window* GetWindow() const { return m_pWindow; };

	private:
		static const int m_MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		int m_WindowWidth;
		int m_WindowHeight;
		SDL_Window* m_pWindow{};
	};
}