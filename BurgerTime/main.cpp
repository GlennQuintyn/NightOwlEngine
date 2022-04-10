
#include <iostream> // std::cout
#include <sstream> // stringstream
#include <memory> // smart pointers
#include <vector>

//self made stuff to have handy
#include "time.h"
#include "logger.h"

#define win32_lean_and_mean
#include <windows.h>

//disabling all warnings that come from this math library
#pragma warning(push)
#pragma warning( disable : 26495 )
#pragma warning( disable : 26812 )
#pragma warning( disable : 26819 )
#pragma warning( disable : 4201 )
#include <glm/glm.hpp>
#include <SDL.h>
#pragma warning (pop)

#include "NightOwlEngine.h"

#pragma region Components importing
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Texture2DComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "TrashTheCacheComponent.h"
#include "LivesComponent.h"
#include "PeterPepper.h"
#include "ScoreComponent.h"
#include "Subject.h"
#include "SpriteComponent.h"
#pragma endregion

#include "BurgerTimeCommands.h"

using namespace dae;

int main(int, char* [])
{
	auto engine = new dae::NightOwlEngine();

	engine->Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

#pragma region LoadGame

	//std::cout << "\n\n-player 1:\n\tButton Y: Lose a life\n\tButton R1: add 25 to score\n\tButton R3: add 50 to score\n\n";
	//std::cout << "-player 2:\n\tButton X: Lose a life\n\tButton L1: add 25 to score\n\tButton L3: add 50 to score\n\n";

	auto& scene = SceneManager::GetInstance().CreateScene("FPS Counter");
	auto& inputmanager = InputManager::GetInstance();

	auto fpsFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 19);
	auto normalfont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);


	auto& backgroundObject = scene.CreateObject("backgroundObject");
	backgroundObject.AddComponent<Texture2DComponent>().SetTexture("background.jpg");


	auto& fpsCounterObj = scene.CreateObject("fpsCounterObj");
	auto& fpstextComponent = fpsCounterObj.AddComponent<TextComponent>();
	auto& fpsComponent = fpsCounterObj.AddComponent<FPSComponent>();
	fpsComponent.SetTextComponent(fpstextComponent);
	fpsCounterObj.SetLocalPosition(5.f, 5.f);
	fpstextComponent.SetFont(fpsFont);
	fpstextComponent.SetTextColor({ 0, 255, 255 });

	//peter pepper test object
	auto& peterPepperObj = scene.CreateObject("peterPepperObj");
	peterPepperObj.AddComponent<PeterPepper>();
	auto& peterSprite = peterPepperObj.AddComponent<SpriteComponent>();
	//peterSprite.SetTexture("Peter_Walking_Down.png");
	peterSprite.SetTexture("Peter_Walking_Up.png");
	peterSprite.Setup(4, 1, 8, 64, 64);

	peterPepperObj.SetLocalPosition(200, 200);
	inputmanager.AddCommand<WalkRightCommand>(PCController::ControllerButton::Button_DPAD_RIGHT, PCController::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkLeftCommand>(PCController::ControllerButton::Button_DPAD_LEFT, PCController::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkUpCommand>(PCController::ControllerButton::Button_DPAD_UP, PCController::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkDownCommand>(PCController::ControllerButton::Button_DPAD_DOWN, PCController::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);

	/*
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
		subje2.AddObserver(scorecomp2);*/




#pragma endregion

	engine->Run();

	engine->Cleanup();
	delete engine;

	return 0;
}