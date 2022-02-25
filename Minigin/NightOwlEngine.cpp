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
#include "FPSComponent.h"
#include "TrashTheCacheComponent.h"

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

	auto fpsFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 19);
	auto normalfont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto fpsCounterObj = std::make_shared<GameObject>();
	auto fpsComponent = fpsCounterObj->AddComponent<dae::FPSComponent>();
	fpsComponent->SetFont(fpsFont);
	fpsComponent->SetPosition(5.0f, 5.f);
	fpsComponent->SetTextColor({ 0, 255, 255 });
	scene.Add(fpsCounterObj);

	//fpsCounterObj->AddComponent<FPSComponent>(textComponent);
	//fpsCounterObj->AddComponent(textComponent, &typeid(textComponent));

	//auto textComponent = new TextComponent();
	/*fpsCounterObj->AddComponent<dae::TextComponent>()->SetFont(font);
	fpsCounterObj->GetComponent<dae::TextComponent>()->SetFpsMode(true);
	fpsCounterObj->GetComponent<dae::TextComponent>()->SetPosition(35.0f, 35.f);*/


	auto logoObject = std::make_shared<GameObject>();
	auto textureComponent = logoObject->AddComponent<Texture2DComponent>();
	textureComponent->SetTexture("logo.png");
	textureComponent->SetPosition(216, 180);
	scene.Add(logoObject);

	auto textObject = std::make_shared<GameObject>();
	auto textComponent = logoObject->AddComponent<TextComponent>();
	textComponent->SetText("Programming 4 Assignment");
	textComponent->SetPosition(80, 20);
	textComponent->SetFont(normalfont);
	scene.Add(textObject);

	auto ImguiTestObject = std::make_shared<GameObject>();
	ImguiTestObject->AddComponent<TrashTheCacheComponent>();
	scene.Add(ImguiTestObject);

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
	//time.SetFPSVolatility(10);

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
		sceneManager.Update();
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