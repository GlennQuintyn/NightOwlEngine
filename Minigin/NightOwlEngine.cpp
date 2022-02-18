#include "NightOwlEnginePCH.h"
#include "NightOwlEngine.h"
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Time.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "GameObject.h"
#include "Scene.h"

#include "Texture2DComponent.h"
#include "TextComponent.h"

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
	auto& scene = SceneManager::GetInstance().CreateScene("FPS Counter");

	auto backgroundObject = std::make_shared<GameObject>();
	backgroundObject->AddComponent<dae::Texture2DComponent>()->SetTexture("background.jpg");
	scene.Add(backgroundObject);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto fpsCounterObj = std::make_shared<GameObject>();
	auto textComponent = fpsCounterObj->AddComponent<dae::TextComponent>();
	textComponent->SetFont(font);
	//todo: change fps mode to a seperate component and add precision of float to as option and maybe add code to make fps more or less volatile
	textComponent->SetFpsMode(true);
	textComponent->SetPosition(35.0f, 35.f);
	textComponent->SetTextColor({ 0, 255, 255 });

	fpsCounterObj->AddComponent<TextComponent>(textComponent);
	scene.Add(fpsCounterObj);

	//auto textComponent = new TextComponent();
	/*fpsCounterObj->AddComponent<dae::TextComponent>()->SetFont(font);
	fpsCounterObj->GetComponent<dae::TextComponent>()->SetFpsMode(true);
	fpsCounterObj->GetComponent<dae::TextComponent>()->SetPosition(35.0f, 35.f);*/


	//go = std::make_shared<GameObject>();
	//go->SetTexture("logo.png");
	//go->SetPosition(216, 180);
	//scene.Add(go);

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
	auto& time = Time::GetInstance();

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
		time.Update(deltaT);

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