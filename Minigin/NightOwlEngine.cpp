#include "NightOwlEnginePCH.h"
#include "NightOwlEngine.h"
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "ColliderManager.h"
#include "GameObject.h"
#include "Scene.h"

#pragma warning(push)
#pragma warning( disable : 26812 )
#pragma warning( disable : 4996 )
#include <steam_api.h>
#pragma warning (pop)

#include "Texture2DComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "TrashTheCacheComponent.h"
//#include "LivesComponent.h"
//#include "PeterPepper.h"
//#include "ScoreComponent.h"
//#include "Subject.h"

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

dae::NightOwlEngine::NightOwlEngine(int windowWidth, int windowHeight)
	:m_WindowWidth{ windowWidth }
	, m_WindowHeight{ windowHeight }
{
}

void dae::NightOwlEngine::Initialize()
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_pWindow = SDL_CreateWindow(
		"BURGERTIME",//"Programming 4 assignment"
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_WindowWidth,//640,//640
		m_WindowHeight,//900,//480
		SDL_WINDOW_OPENGL
	);

	if (m_pWindow == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_pWindow);
}

/**
 * Code constructing the scene world starts here
 */
 /*
 void dae::NightOwlEngine::LoadGame()
 {
	 std::cout << "\n\n-player 1:\n\tButton Y: Lose a life\n\tButton R1: add 25 to score\n\tButton R3: add 50 to score\n\n";
	 std::cout << "-player 2:\n\tButton X: Lose a life\n\tButton L1: add 25 to score\n\tButton L3: add 50 to score\n\n";

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

	 //fpsCounterObj->AddComponent<FPSComponent>(textComponent);
	 //fpsCounterObj->AddComponent(textComponent, &typeid(textComponent));
	 //auto textComponent = new TextComponent();
	 //fpsCounterObj->AddComponent<dae::TextComponent>()->SetFont(font);
	 //fpsCounterObj->GetComponent<dae::TextComponent>()->SetFpsMode(true);
	 //fpsCounterObj->GetComponent<dae::TextComponent>()->SetPosition(35.0f, 35.f);

	 auto& logoObject = scene.CreateObject("logoObject");
	 auto& textureComponent = logoObject.AddComponent<Texture2DComponent>();
	 textureComponent.SetTexture("logo.png");
	 logoObject.SetLocalPosition(216, 180);

	 auto& textObject = scene.CreateObject("textObject");
	 auto& textComponent = textObject.AddComponent<TextComponent>();
	 textComponent.SetText("Programming 4 Assignment");
	 textObject.SetLocalPosition(80, 20);
	 textComponent.SetFont(normalfont);


	 //
	 //onbserver testing code
	 //

	 //1st "player"
	 auto& livesObject1 = scene.CreateObject("livesObject1");
	 auto& livestextcomp1 = livesObject1.AddComponent<TextComponent>();
	 auto& livescomp1 = livesObject1.AddComponent<LivesComponent>();
	 livestextcomp1.SetFont(fpsFont);
	 livestextcomp1.SetTextColor(255, 255, 0);
	 livescomp1.SetTextComponent(livestextcomp1);
	 livesObject1.SetLocalPosition(5, 300);

	 auto& scoreObject1 = scene.CreateObject("scoreObject1");
	 auto& scoretextcomp1 = scoreObject1.AddComponent<TextComponent>();
	 auto& scorecomp1 = scoreObject1.AddComponent<ScoreComponent>();
	 scoretextcomp1.SetFont(fpsFont);
	 scoretextcomp1.SetTextColor(255, 255, 0);
	 scorecomp1.SetTextComponent(scoretextcomp1);
	 scoreObject1.SetLocalPosition(5, 325);


	 auto& peterPepperObj1 = scene.CreateObject("peterPepperObj1");
	 auto& peterPepperComp1 = peterPepperObj1.AddComponent<PeterPepper>();
	 auto& subje1 = peterPepperComp1.GetSubject();
	 subje1.AddObserver(livescomp1);
	 subje1.AddObserver(scorecomp1);





	 //2nd "player"
	 auto& livesObject2 = scene.CreateObject("livesObject2");
	 auto& livestextcomp2 = livesObject2.AddComponent<TextComponent>();
	 auto& livescomp2 = livesObject2.AddComponent<LivesComponent>();
	 livestextcomp2.SetFont(fpsFont);
	 livestextcomp2.SetTextColor(0, 255, 0);
	 livescomp2.SetTextComponent(livestextcomp2);
	 livesObject2.SetLocalPosition(5, 400);

	 auto& scoreObject2 = scene.CreateObject("scoreObject2");
	 auto& scoretextcomp2 = scoreObject2.AddComponent<TextComponent>();
	 auto& scorecomp2 = scoreObject2.AddComponent<ScoreComponent>();
	 scoretextcomp2.SetFont(fpsFont);
	 scoretextcomp2.SetTextColor(0, 255, 0);
	 scorecomp2.SetTextComponent(scoretextcomp2);
	 scoreObject2.SetLocalPosition(5, 425);

	 auto& peterPepperObj2 = scene.CreateObject("peterPepperObj2");
	 auto& peterPepperComp2 = peterPepperObj2.AddComponent<PeterPepper>();
	 peterPepperComp2.SetDeathButton(PCController::ControllerButton::Button_Square);
	 peterPepperComp2.SetObjectFellButton(PCController::ControllerButton::Button_L_SHOULDER);
	 peterPepperComp2.SetPepperEnemyButton(PCController::ControllerButton::Button_L_THUMB);
	 auto& subje2 = peterPepperComp2.GetSubject();
	 subje2.AddObserver(livescomp2);
	 subje2.AddObserver(scorecomp2);




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

 }
 */

void dae::NightOwlEngine::Cleanup()
{
	Renderer::GetInstance().Destroy();
	ServiceLocator::RegisterSoundSystem(nullptr);
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
	SDL_Quit();
}

void dae::NightOwlEngine::Run()
{
	//Initialize();

	//// tell the resource manager where it can find the game data
	//ResourceManager::GetInstance().Init("../Data/");

	//LoadGame();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& time = Time::GetInstance();
	auto& collider = ColliderManager::GetInstance();
	//auto& SS = ServiceLocator::GetSS();

	sceneManager.LateInit();

	bool doContinue = true;
	auto prevTime = chrono::high_resolution_clock::now();
	float lag = 0.0f;

	//float timer{};

	while (doContinue)
	{
		SteamAPI_RunCallbacks();

		const auto currentTime = chrono::high_resolution_clock::now();
		const float deltaT = chrono::duration<float>(currentTime - prevTime).count();
		prevTime = currentTime;
		lag += deltaT;

		time.Update(deltaT);
		doContinue = input.ProcessInput();
		sceneManager.Update();

		collider.Update();
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
}