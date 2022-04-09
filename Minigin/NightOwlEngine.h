#pragma once
struct SDL_Window;

namespace dae
{
	class GameObject;

	class NightOwlEngine
	{
	public:
		void Initialize();
		//void LoadGame() /*const*/;
		void Run();
		void Cleanup();

	private:
		static const int m_MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		/*Test code for input and transfrom code
		GameObject* temp;*/
		SDL_Window* m_Window{};
	};
}