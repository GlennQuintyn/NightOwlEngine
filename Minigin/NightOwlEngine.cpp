#include "NightOwlEnginePCH.h"
#include "NightOwlEngine.h"
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "GameObject.h"
#include "Scene.h"

using namespace std;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void dae::NightOwlEngine::Initialize()
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);

	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::NightOwlEngine::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto fpsCounterObj = std::make_shared<GameObject>().;
	//fpsCounterObj->AddComponent<Tex>
	
	//auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	//auto go = std::make_shared<GameObject>();
	//go->SetTexture("background.jpg");
	//scene.Add(go);

	//go = std::make_shared<GameObject>();
	//go->SetTexture("logo.png");
	//go->SetPosition(216, 180);
	//scene.Add(go);

	//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = std::make_shared<TextObject>("Programming 4 Assignment", font);
	//to->SetPosition(80, 20);
	//scene.Add(to);
}

void dae::NightOwlEngine::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::NightOwlEngine::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	bool doContinue = true;
	auto prevTime = chrono::high_resolution_clock::now();
	float lag = 0.0f;

	while (doContinue)
	{
		const auto currentTime = chrono::high_resolution_clock::now();
		const float deltaT = chrono::duration<float>(currentTime - prevTime).count();
		prevTime = currentTime;
		lag += deltaT;

		doContinue = input.ProcessInput();
		sceneManager.Update(deltaT);

		//used for physics 
		while (lag >= m_MsPerFrame)
		{
			sceneManager.FixedUpdate(m_MsPerFrame);
			lag -= m_MsPerFrame;
		}

		//todo: add some form of late update function

		renderer.Render();

		//make thread sleep to save on PC resources
		const auto sleepTime = currentTime + chrono::milliseconds(m_MsPerFrame) - chrono::high_resolution_clock::now();
		this_thread::sleep_for(sleepTime);
	}

	Cleanup();
}