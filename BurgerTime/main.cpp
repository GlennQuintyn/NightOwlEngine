#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "BurgerTimePCH.h"

//#include <iostream> // std::cout
//#include <sstream> // stringstream
//#include <memory> // smart pointers
//#include <vector>
//#include <string>
//
////self made stuff to have handy
//#include "time.h"
//#include "logger.h"
//
////#define win32_lean_and_mean
////#include <windows.h>
//
////disabling all warnings that come from this math library
//#pragma warning(push)
//#pragma warning( disable : 26495 )
//#pragma warning( disable : 26812 )
//#pragma warning( disable : 26819 )
//#pragma warning( disable : 4201 )
//#include <glm/glm.hpp>
//#include <SDL.h>
//#pragma warning (pop)

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
#include "HiScoreComponent.h"
#include "LivesComponent.h"
#include "PepperCountComponent.h"
#pragma endregion

#include "BurgerTimeCommands.h"

#include "ServiceLocator.h"
#include "SDLSoundSystem.h"

using namespace dae;

int main(int, char* [])
{
	auto engine = new dae::NightOwlEngine();

	engine->Initialize();

	// tell the resource manager where it can find the game data
	ResourceManager::GetInstance().Init("../Data/");

#pragma region LoadGame

	//std::cout << "\n\n-player 1:\n\tButton Y: Lose a life\n\tButton R1: add 25 to score\n\tButton R3: add 50 to score\n\n";
	//std::cout << "-player 2:\n\tButton X: Lose a life\n\tButton L1: add 25 to score\n\tButton L3: add 50 to score\n\n";

	auto& scene = SceneManager::GetInstance().CreateScene("FPS Counter");
	auto& inputmanager = InputManager::GetInstance();

	auto fpsFont = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 19);
	//auto normalfont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	int windowW{}, windowH{};
	SDL_GetWindowSize(engine->GetWindow(), &windowW, &windowH);

	auto& backgroundObject = scene.CreateObject("backgroundObject");
	backgroundObject.AddComponent<Texture2DComponent>().SetTexture("background_adj.jpg");


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
	peterSprite.SetTexture("peter/Peter_Walking_Up.png");
	peterSprite.Setup(4, 1, 16, 50, 50);


	peterPepperObj.SetLocalPosition(200, 200);
	inputmanager.AddCommand<WalkRightCommand>(PCController::ControllerButton::Button_DPAD_RIGHT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkRightCommand>(InputManager::KeyboardKey::Key_D, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);

	inputmanager.AddCommand<WalkLeftCommand>(PCController::ControllerButton::Button_DPAD_LEFT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkLeftCommand>(InputManager::KeyboardKey::Key_A, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);

	inputmanager.AddCommand<WalkUpCommand>(PCController::ControllerButton::Button_DPAD_UP, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkUpCommand>(InputManager::KeyboardKey::Key_W, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);

	inputmanager.AddCommand<WalkDownCommand>(PCController::ControllerButton::Button_DPAD_DOWN, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkDownCommand>(InputManager::KeyboardKey::Key_S, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);


#pragma region UI/SCORE
	int uiFontSize{ 20 };
	auto uiFont = ResourceManager::GetInstance().LoadFont("Fonts/BurgerTime_Font.otf", uiFontSize);

	//
	//Static UI
	//
	auto& oneUpTextObject = scene.CreateObject("oneUpTextObject");
	/*auto& oneUpTextcomp = */oneUpTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 255, 0, 0 }, "1UP");
	//oneUpTextcomp.SetFont(uiFont);
	//oneUpTextcomp.SetTextColor(255, 0, 0);
	//oneUpTextcomp.SetText("1UP");
	oneUpTextObject.SetLocalPosition(float(uiFontSize * 4), 5);

	auto& hiScoreTextObject = scene.CreateObject("hiScoreTextObject");
	/*auto& hiScoreTextcomp = */hiScoreTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 255, 0, 0 }, "HI-SCORE");
	//hiScoreTextcomp.SetFont(uiFont);
	//hiScoreTextcomp.SetTextColor(255, 0, 0);
	//hiScoreTextcomp.SetText("HI-SCORE");
	hiScoreTextObject.SetLocalPosition(float(uiFontSize * 4) + (uiFontSize * 5), 5);

	auto& pepperTextObject = scene.CreateObject("pepperTextObject");
	/*auto& pepperTextcomp = */pepperTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 0, 255, 0 }, "PEPPER");
	//pepperTextcomp.SetFont(uiFont);
	//pepperTextcomp.SetTextColor(0, 255, 0);
	//pepperTextcomp.SetText("PEPPER");
	pepperTextObject.SetLocalPosition(float(windowW - (uiFontSize * 7)), 5);

	//
	//Dynamic UI
	//
	auto& peterLivesObj = scene.CreateObject("peterLivesObj");
	peterLivesObj.AddComponent<LivesComponent>().SetTexture("UI/Peter_Life_Icon.png", 3.25f, 3.25f);
	peterLivesObj.SetLocalPosition(10.f, windowH - 40.f);

	auto& scoreObject = scene.CreateObject("scoreObject");
	auto& scoretextcomp = scoreObject.AddComponent<TextComponent>();
	scoretextcomp.Setup(uiFont);
	scoreObject.AddComponent<ScoreComponent>().SetTextComponent(scoretextcomp);
	scoreObject.SetLocalPosition(uiFontSize * 7.f, uiFontSize * 1.5f);

	//TODO: hi score component should read hi-score from file and observe the score object,
	//if the score components value is bigger than the current high score update it

	auto& hiScoreObject = scene.CreateObject("scoreObject");
	auto& hiScoretextcomp = hiScoreObject.AddComponent<TextComponent>();
	hiScoretextcomp.Setup(uiFont);
	hiScoreObject.AddComponent<HiScoreComponent>().SetTextComponent(hiScoretextcomp);
	hiScoreObject.SetLocalPosition(uiFontSize * 15.f, uiFontSize * 1.5f);

	auto& pepperCountObject = scene.CreateObject("pepperCountObject");
	auto& pepperCountComp = pepperCountObject.AddComponent<TextComponent>();
	pepperCountComp.Setup(uiFont);
	pepperCountObject.AddComponent<PepperCountComponent>().SetTextComponent(pepperCountComp);
	pepperCountObject.SetLocalPosition(windowW - (uiFontSize * 2.f), uiFontSize * 1.5f);
#pragma endregion



	ServiceLocator::RegisterSoundSystem(new SDLSoundSystem("../Data/Sound/"));
	ServiceLocator::GetSS().RegisterSound("test.wav", 0);
	ServiceLocator::GetSS().RegisterSound("test2.wav", 1);
	ServiceLocator::GetSS().RegisterSound("test3.wav", 2);
	ServiceLocator::GetSS().PlaySFX(0, 40);
	ServiceLocator::GetSS().PlaySFX(1, 50);
	ServiceLocator::GetSS().PlaySFX(2, 80);

	//auto rendereerere = Renderer::GetInstance().GetSDLRenderer();
	////SDL_renderD
	//SDL_RenderDrawLine(rendereerere, 50, 50, 150, 150);
	//
	//SDL_RenderPresent(rendereerere);






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