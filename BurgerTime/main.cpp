#include "BurgerTimePCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "NightOwlEngine.h"

#include <ResourceManager.h>
#include <SceneManager.h>
#include "InputManager.h"

#include <GameObject.h>
#include <Texture2DComponent.h>
#include <TextComponent.h>

#include "MainMenuComponent.h"
#include "ButtonComponent.h"

#include "BurgerTimeCommands.h"
#include <ServiceLocator.h>
#include <SDLSoundSystem.h>

/*
#pragma region Components importing
#include "Renderer.h"

#include "Scene.h"

#include "FPSComponent.h"

#include "Subject.h"
#include "SpriteComponent.h"
#include "RectColliderComponent.h"
#include "SpriteManagerComponent.h"

#include "MovementComponent.h"
#include "PeterPepper.h"
#include "MrHotDog.h"
#include "MrEgg.h"
#include "MrPickle.h"
#include "EnemyControllerComponent.h"

#include "GameManager.h"

#include "PepperComponent.h"

#include "PepperCountComponent.h"
#include "HiScoreComponent.h"
#include "LivesComponent.h"
#include "LivesComponent.h"
#include "ScoreComponent.h"

#include "LadderComponent.h"
#include "WalkingPlatformComponent.h"

#include "IngredientComponent.h"
#include "PlateComponent.h"
#pragma endregion
*/

void LoadMainMenu(dae::NightOwlEngine* pEngine);

using namespace dae;

int main(int, char* [])
{
	auto engine = new dae::NightOwlEngine(640, 900, "BURGERTIME");

	engine->Initialize();

	// tell the resource manager where it can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	//loads the main menu and that takes care of the rest of the game
	LoadMainMenu(engine);

	//TODO: remove the code below
	/*sound system testing code
	//ServiceLocator::RegisterSoundSystem(new LoggingSoundSystem(new SDLSoundSystem("../Data/Sound/")));
	//ServiceLocator::RegisterSoundSystem(new SDLSoundSystem("../Data/Sound/"));
	//ServiceLocator::GetSS().RegisterSound("test.wav", 0);
	//ServiceLocator::GetSS().RegisterSound("test2.wav", 1);
	//ServiceLocator::GetSS().RegisterSound("test3.wav", 2);
	//ServiceLocator::GetSS().PlaySFX(0, 40);
	//ServiceLocator::GetSS().PlaySFX(1, 50);
	//ServiceLocator::GetSS().PlaySFX(2, 80);



		//
		//onbserver testing code
		//

		//1st "player"
		auto& livesObject1 = sceneLevel1.CreateObject("livesObject1");
		auto& livestextcomp1 = livesObject1.AddComponent<TextComponent>();
		auto& livescomp1 = livesObject1.AddComponent<LivesComponent>();
		livestextcomp1.SetFont(fpsFont);
		livestextcomp1.SetTextColor(255, 255, 0);
		livescomp1.SetTextComponent(livestextcomp1);
		livesObject1.SetLocalPosition(5, 300);

		auto& scoreObject1 = sceneLevel1.CreateObject("scoreObject1");
		auto& scoretextcomp1 = scoreObject1.AddComponent<TextComponent>();
		auto& scorecomp1 = scoreObject1.AddComponent<ScoreComponent>();
		scoretextcomp1.SetFont(fpsFont);
		scoretextcomp1.SetTextColor(255, 255, 0);
		scorecomp1.SetTextComponent(scoretextcomp1);
		scoreObject1.SetLocalPosition(5, 325);


		auto& peterPepperObj1 = sceneLevel1.CreateObject("peterPepperObj1");
		auto& peterPepperComp1 = peterPepperObj1.AddComponent<PeterPepper>();
		auto& pettercollidersubje = peterPepperComp1.GetSubject();
		pettercollidersubje.AddObserver(livescomp1);
		pettercollidersubje.AddObserver(scorecomp1);





		//2nd "player"
		auto& livesObject2 = sceneLevel1.CreateObject("livesObject2");
		auto& livestextcomp2 = livesObject2.AddComponent<TextComponent>();
		auto& livescomp2 = livesObject2.AddComponent<LivesComponent>();
		livestextcomp2.SetFont(fpsFont);
		livestextcomp2.SetTextColor(0, 255, 0);
		livescomp2.SetTextComponent(livestextcomp2);
		livesObject2.SetLocalPosition(5, 400);

		auto& scoreObject2 = sceneLevel1.CreateObject("scoreObject2");
		auto& scoretextcomp2 = scoreObject2.AddComponent<TextComponent>();
		auto& scorecomp2 = scoreObject2.AddComponent<ScoreComponent>();
		scoretextcomp2.SetFont(fpsFont);
		scoretextcomp2.SetTextColor(0, 255, 0);
		scorecomp2.SetTextComponent(scoretextcomp2);
		scoreObject2.SetLocalPosition(5, 425);

		auto& peterPepperObj2 = sceneLevel1.CreateObject("peterPepperObj2");
		auto& peterPepperComp2 = peterPepperObj2.AddComponent<PeterPepper>();
		peterPepperComp2.SetDeathButton(PCController::ControllerButton::Button_Square);
		peterPepperComp2.SetObjectFellButton(PCController::ControllerButton::Button_L_SHOULDER);
		peterPepperComp2.SetPepperEnemyButton(PCController::ControllerButton::Button_L_THUMB);
		auto& subje2 = peterPepperComp2.GetSubject();
		subje2.AddObserver(livescomp2);
		subje2.AddObserver(scorecomp2);

	*/

	engine->Run();

	engine->Cleanup();
	delete engine;

	return 0;
}

void LoadMainMenu(dae::NightOwlEngine* pEngine)
{
	ServiceLocator::RegisterSoundSystem(new SDLSoundSystem("../Data/Sound/"));
	ServiceLocator::GetSS().RegisterSound("EnemyCrushed.wav", static_cast<int>(SoundIndices::EnemyCrushed));
	ServiceLocator::GetSS().RegisterSound("IngredienDropped.wav", static_cast<int>(SoundIndices::IngredienDropped));
	ServiceLocator::GetSS().RegisterSound("LevelWon.wav", static_cast<int>(SoundIndices::LevelWon));
	ServiceLocator::GetSS().RegisterSound("PepperThrown.wav", static_cast<int>(SoundIndices::PepperThrown));
	ServiceLocator::GetSS().RegisterSound("PlayerDied.wav", static_cast<int>(SoundIndices::PlayerDied));
	ServiceLocator::GetSS().RegisterSound("PlayerHit.wav", static_cast<int>(SoundIndices::PlayerHit));


	auto& sceneMainMenu = SceneManager::GetInstance().CreateScene("MainMenu");

	int windowW{}, windowH{};
	SDL_GetWindowSize(pEngine->GetWindow(), &windowW, &windowH);

	auto& backgroundObject = sceneMainMenu.CreateObject("backgroundObject");
	backgroundObject.AddComponent<Texture2DComponent>().SetTexture("Level/background_adj.jpg");

	auto& logoObject = sceneMainMenu.CreateObject("burgerTimeLogoObject");
	logoObject.AddComponent<Texture2DComponent>().SetTexture("Level/BurgerTime_Logo.png");
	logoObject.SetLocalPosition(140.f, 160.f);

#pragma region Buttons
	auto& singlePlayerBtnObject = sceneMainMenu.CreateObject("singlePlayerBtnObject");
	auto& singlePlayerBtncmpt = singlePlayerBtnObject.AddComponent<ButtonComponent>();
	singlePlayerBtncmpt.SetTextureNormal("UI/Buttons/SinglePlayer_Normal.png");
	singlePlayerBtncmpt.SetTextureSelected("UI/Buttons/SinglePlayer_Selected.png");
	singlePlayerBtnObject.SetLocalPosition(111.f, 410.f);

	auto& coopBtnObject = sceneMainMenu.CreateObject("coopBtnObject");
	auto& coopBtncmpt = coopBtnObject.AddComponent<ButtonComponent>();
	coopBtncmpt.SetTextureNormal("UI/Buttons/Coop_Normal.png");
	coopBtncmpt.SetTextureSelected("UI/Buttons/Coop_Selected.png");
	coopBtnObject.SetLocalPosition(241.f, 490.f);

	auto& versusBtnObject = sceneMainMenu.CreateObject("versusBtnObject");
	auto& versusBtncmpt = versusBtnObject.AddComponent<ButtonComponent>();
	versusBtncmpt.SetTextureNormal("UI/Buttons/Versus_Normal.png");
	versusBtncmpt.SetTextureSelected("UI/Buttons/Versus_Selected.png");
	versusBtnObject.SetLocalPosition(224.f, 569.f);
#pragma endregion

	auto& mainMenuObject = sceneMainMenu.CreateObject("mainMenuObject");
	auto& mainMenuCmpt = mainMenuObject.AddComponent<MainMenuComponent>();
	mainMenuCmpt.AddButton(&singlePlayerBtncmpt);
	mainMenuCmpt.AddButton(&coopBtncmpt);
	mainMenuCmpt.AddButton(&versusBtncmpt);
	mainMenuCmpt.SetEngine(pEngine);

#pragma region MenuCommands
	auto& inputmanager = InputManager::GetInstance();
	inputmanager.AddCommand<NextButtonCommand>(PCController::ControllerButton::Button_DPAD_DOWN, InputManager::ButtonPressState::OnPressed).SetMainMenuCmpt(&mainMenuCmpt);
	inputmanager.AddCommand<NextButtonCommand>(InputManager::KeyboardKey::Key_ARROW_DOWN, InputManager::ButtonPressState::OnPressed).SetMainMenuCmpt(&mainMenuCmpt);
	inputmanager.AddCommand<PreviousButtonCommand>(PCController::ControllerButton::Button_DPAD_UP, InputManager::ButtonPressState::OnPressed).SetMainMenuCmpt(&mainMenuCmpt);
	inputmanager.AddCommand<PreviousButtonCommand>(InputManager::KeyboardKey::Key_ARROW_UP, InputManager::ButtonPressState::OnPressed).SetMainMenuCmpt(&mainMenuCmpt);

	inputmanager.AddCommand<MouseButtonCommand>(static_cast<int>(InputManager::MouseButtons::Button_LEFT), InputManager::ButtonPressState::OnPressed).SetMainMenuCmpt(&mainMenuCmpt);

	inputmanager.AddCommand<PressSelectedButtonCommand>(PCController::ControllerButton::Button_Cross, InputManager::ButtonPressState::OnPressed).SetMainMenuCmpt(&mainMenuCmpt);
	inputmanager.AddCommand<PressSelectedButtonCommand>(InputManager::KeyboardKey::Key_ENTER, InputManager::ButtonPressState::OnPressed).SetMainMenuCmpt(&mainMenuCmpt);
#pragma endregion

	const int nameFontSize{ 8 };
	auto nameFont = ResourceManager::GetInstance().LoadFont("Fonts/BurgerTime_Font.otf", nameFontSize);

	auto& nameObject = sceneMainMenu.CreateObject("nameObject");
	auto& nameTextCmpt = nameObject.AddComponent<TextComponent>();
	nameTextCmpt.Setup(nameFont, { 255,255,255,255 }, "Glenn Quintyn");
	nameObject.SetLocalPosition(10.f, windowH - nameFontSize - 5.f);


}