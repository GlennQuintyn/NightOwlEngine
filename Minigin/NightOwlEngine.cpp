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
void dae::NightOwlEngine::LoadGame() /*const*/
{
	auto& scene = SceneManager::GetInstance().CreateScene("FPS Counter");

	auto& backgroundObject = scene.CreateObject("backgroundObject");
	backgroundObject.AddComponent<dae::Texture2DComponent>().SetTexture("background.jpg");

	auto fpsFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 19);
	auto normalfont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto& fpsCounterObj = scene.CreateObject("fpsCounterObj");
	auto& fpstextComponent = fpsCounterObj.AddComponent<TextComponent>();
	auto& fpsComponent = fpsCounterObj.AddComponent<FPSComponent>();
	fpsComponent.SetTextComponent(fpstextComponent);
	fpsCounterObj.SetLocalPosition(5.f, 5.f);
	fpstextComponent.SetFont(fpsFont);
	fpstextComponent.SetTextColor({ 0, 255, 255 });
	//scene.Add(fpsCounterObj);

	/*fpsCounterObj->AddComponent<FPSComponent>(textComponent);
	fpsCounterObj->AddComponent(textComponent, &typeid(textComponent));
	auto textComponent = new TextComponent();
	fpsCounterObj->AddComponent<dae::TextComponent>()->SetFont(font);
	fpsCounterObj->GetComponent<dae::TextComponent>()->SetFpsMode(true);
	fpsCounterObj->GetComponent<dae::TextComponent>()->SetPosition(35.0f, 35.f);*/

	auto& logoObject = scene.CreateObject("logoObject");
	auto& textureComponent = logoObject.AddComponent<Texture2DComponent>();
	textureComponent.SetTexture("logo.png");
	logoObject.SetLocalPosition(216, 180);
	//scene.Add(logoObject);

	auto& textObject = scene.CreateObject("textObject");
	auto& textComponent = textObject.AddComponent<TextComponent>();
	textComponent.SetText("Programming 4 Assignment");
	textObject.SetLocalPosition(80, 20);
	textComponent.SetFont(normalfont);
	//scene.Add(textObject);

	//Test code for input and transfrom code
	//temp = &logoObject;
	//scene.CreateObject("test")
	//temp = new GameObject();

	//auto ImguiTestObject = std::make_unique<GameObject>();
	//ImguiTestObject->AddComponent<TrashTheCacheComponent>();
	//scene.Add(ImguiTestObject);
	//return;

	//testing area [[maybe_unused]]
	//auto& rootobj = scene.CreateObject("rootobj");
	//auto child1 = rootobj.CreateAddChild("child1");
	//auto child2 = rootobj.CreateAddChild("child2");
	//auto child3 = rootobj.CreateAddChild("child3");
	//[[maybe_unused]] auto child4 = rootobj.CreateAddChild("child3");//should give dupplicate child name warning and rename it to smt new
	//
	//[[maybe_unused]] auto grandchild1 = child1->CreateAddChild("grandchild1");
	//[[maybe_unused]] auto grandchild2 = child1->CreateAddChild("grandchild1");//should give same duplicate child name warning
	//[[maybe_unused]] auto grandchild3 = child2->CreateAddChild("grandchild1");
	//
	//grandchild3->SetParent(child1);//should give error of duplicate child name
	//grandchild3->SetParent(child3);
	////grandchild3->RemoveParent();//object should now be part of the scene and not float free
	////grandchild3->SetName("");
	//grandchild3->SetParent(nullptr);


	//auto yesyes = std::make_unique<GameObject>("grandchild1", &scene);
	//child1->AdoptChild(child2);//should create error (childing twice)
	//child1->AdoptChild(std::move(yesyes));
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

	sceneManager.LateInit();

	bool doContinue = true;
	auto prevTime = chrono::high_resolution_clock::now();
	float lag = 0.0f;



	while (doContinue)
	{
		const auto currentTime = chrono::high_resolution_clock::now();
		const float deltaT = chrono::duration<float>(currentTime - prevTime).count();
		prevTime = currentTime;
		lag += deltaT;

		time.Update(deltaT);
		doContinue = input.ProcessInput();
		sceneManager.Update();

		/*Test code for input and transfrom code
		auto pos = temp->GetWorldPosition();
		if (input.IsPressed(PCController::ControllerButton::Button_Cross))
		{
			pos += glm::vec2{ 0,1 };
		}
		if (input.IsPressed(PCController::ControllerButton::Button_Circle))
		{
			pos += glm::vec2{ 1,0 };
		}
		if (input.IsPressed(PCController::ControllerButton::Button_Triangle))
		{
			pos += glm::vec2{ 0, -1 };
		}
		if (input.IsPressed(PCController::ControllerButton::Button_Square))
		{
			pos += glm::vec2{ -1,0 };
		}
		temp->SetLocalPosition(pos);*/

		//used for physics 
		while (lag >= m_MsPerFrame)
		{
			sceneManager.FixedUpdate(m_MsPerFrame);
			lag -= m_MsPerFrame;
		}

		sceneManager.LateUpdate();

		renderer.Render();

		//make thread sleep to save on PC resources
		const auto sleepTime = currentTime + chrono::milliseconds(m_MsPerFrame) - chrono::high_resolution_clock::now();
		this_thread::sleep_for(sleepTime);
	}

	Cleanup();
}