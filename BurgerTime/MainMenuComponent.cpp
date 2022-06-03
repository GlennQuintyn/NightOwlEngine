#include "BurgerTimePCH.h"
#include "MainMenuComponent.h"

#include <NightOwlEngine.h>

#pragma region Components importing
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include "Scene.h"
#include "GameObject.h"

#include "Texture2DComponent.h"
#include "TextComponent.h"
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

#include "BurgerTimeCommands.h"//TODO: SHOULD BE uncommented again
#include "ServiceLocator.h"
#include "SDLSoundSystem.h"

dae::MainMenuComponent::MainMenuComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_pEngine{ nullptr }
	, m_SelectedButtonIndex{ 0 }
	, m_GameMode{ GameMode::MainMenu }
	, m_ScenesLoaded{ false }
{
}

void dae::MainMenuComponent::SelectNextButton()
{
	if (m_GameMode != GameMode::MainMenu)
		return;

	m_pButtons[m_SelectedButtonIndex]->SetSelectedState(false);

	++m_SelectedButtonIndex;
	if (m_SelectedButtonIndex >= m_pButtons.size())
		m_SelectedButtonIndex = 0;

	m_pButtons[m_SelectedButtonIndex]->SetSelectedState(true);
}

void dae::MainMenuComponent::SelectPreviousButton()
{
	if (m_GameMode != GameMode::MainMenu)
		return;

	m_pButtons[m_SelectedButtonIndex]->SetSelectedState(false);

	--m_SelectedButtonIndex;
	if (m_SelectedButtonIndex < 0)
		m_SelectedButtonIndex = static_cast<int>(m_pButtons.size()) - 1;

	m_pButtons[m_SelectedButtonIndex]->SetSelectedState(true);
}

void dae::MainMenuComponent::CheckIfMouseInsideButton()
{
	if (m_GameMode != GameMode::MainMenu)
		return;

	const auto& mousePos = InputManager::GetInstance().GetMousePosition();
	if (m_pButtons[m_SelectedButtonIndex]->IsMouseInsideOfButton(mousePos))
	{
		//actually pressed the selected the button
		PressSelectedButton();
	}
}

void dae::MainMenuComponent::PressSelectedButton()
{
	//can just SinglePlayer = 0, Coop = 1, Versus = 2
	m_GameMode = static_cast<GameMode>(m_SelectedButtonIndex);

	//loading levels
	LoadLevel1();
	LoadLevel2();
	LoadLevel3();

	//once all scenes have been loaded in with correct game mode
	auto& scene = SceneManager::GetInstance();
	scene.GotoNextScene();
	scene.LateInit();//intialize the new scene
	m_ScenesLoaded = true;//so that they objects aren't added multiple times to their respected scene
}

void dae::MainMenuComponent::LateInit()
{
	if (m_pButtons.size() > 0)
	{
		m_pButtons[m_SelectedButtonIndex]->SetSelectedState(true);
	}

	m_GameMode = GameMode::MainMenu;
}

void dae::MainMenuComponent::Update()
{
	if (m_GameMode != GameMode::MainMenu)
		return;

	const auto& mousePos = InputManager::GetInstance().GetMousePosition();
	int index{};

	for (auto& button : m_pButtons)
	{
		if (button->IsMouseInsideOfButton(mousePos))
		{
			m_pButtons[m_SelectedButtonIndex]->SetSelectedState(false);
			m_SelectedButtonIndex = index;
			m_pButtons[m_SelectedButtonIndex]->SetSelectedState(true);
		}
		++index;
	}
}

void dae::MainMenuComponent::LoadLevel1()
{

#pragma region LoadGame
	auto& sceneLevel1 = SceneManager::GetInstance().CreateScene("Level1");
	auto& inputmanager = InputManager::GetInstance();

	auto fpsFont = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 19);

	int windowW{}, windowH{};
	SDL_GetWindowSize(m_pEngine->GetWindow(), &windowW, &windowH);

	if (!m_ScenesLoaded)
	{

	}

	auto& backgroundObject = sceneLevel1.CreateObject("backgroundObject");
	backgroundObject.AddComponent<Texture2DComponent>().SetTexture("Level/Level1.png");

	auto& fpsCounterObj = sceneLevel1.CreateObject("fpsCounterObj");
	auto& fpstextComponent = fpsCounterObj.AddComponent<TextComponent>();
	auto& fpsComponent = fpsCounterObj.AddComponent<FPSComponent>();
	fpsComponent.SetTextComponent(fpstextComponent);
	fpsCounterObj.SetLocalPosition(5.f, 5.f);
	fpstextComponent.SetFont(fpsFont);
	fpstextComponent.SetTextColor({ 0, 255, 255 });


	auto& gameManagerObj = sceneLevel1.CreateObject("gameManagerObj");
	auto& gameManagercmpt = gameManagerObj.AddComponent<GameManager>();
	gameManagercmpt.SetMaxFullPlateCount(4);

	//pepper object to throw on enemies
	auto& pepperObj = sceneLevel1.CreateObject("pepperObj");
	auto& pepperCmpt = pepperObj.AddComponent<PepperComponent>();
	auto& pepperSpriteManager = pepperObj.AddComponent<SpriteManagerComponent>();
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Left.png", 4, 1, 4, 45, 45);
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Right.png", 4, 1, 4, 45, 45);
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Up.png", 4, 1, 4, 45, 45);
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Down.png", 4, 1, 4, 45, 45);
	auto& pepperCollider = pepperObj.AddComponent<RectColliderComponent>();
	pepperCollider.Init({ 0,0,45,45 }, -100, true);
	pepperObj.SetLocalPosition(-100.f, -100.f);
	pepperCmpt.SetResetPos(-100.f, -100.f);
	pepperCmpt.SetSpriteDuration(1.f);

	auto& peppercollidersubje = pepperCollider.GetSubject();
	peppercollidersubje.AddObserver(pepperCmpt);

	//sally salt
	//sally salt
	//sally salt

	//peter pepper test object
	auto& peterPepperObj = sceneLevel1.CreateObject("peterPepperObj");
	auto& petercmpt = peterPepperObj.AddComponent<PeterPepper>();
	auto& peterSpriteManager = peterPepperObj.AddComponent<SpriteManagerComponent>();
	peterSpriteManager.AddSprite("Charachters/peter/Peter_Idle.png", 1, 1, 0, 45, 45);
	peterSpriteManager.AddSprite("Charachters/peter/Peter_Walking_Left.png", 4, 1, 16, 45, 45);
	peterSpriteManager.AddSprite("Charachters/peter/Peter_Walking_Right.png", 4, 1, 16, 45, 45);
	peterSpriteManager.AddSprite("Charachters/peter/Peter_Walking_Up.png", 4, 1, 16, 45, 45);
	peterSpriteManager.AddSprite("Charachters/peter/Peter_Walking_Down.png", 4, 1, 16, 45, 45);
	peterSpriteManager.AddSprite("Charachters/peter/Peter_Death.png", 6, 1, 2, 45, 45);
	peterSpriteManager.AddSprite("Charachters/peter/Peter_Won.png", 2, 1, 2, 45, 45);

	auto& peterCollider = peterPepperObj.AddComponent<RectColliderComponent>();
	peterCollider.Init({ 0,0,45,45 }, -1, true);
	peterPepperObj.AddComponent<MovementComponent>();
	petercmpt.SetSpawnLocation(298.f, 563.f);
	peterPepperObj.SetLocalPosition(298.f, 563.f);

	auto& pettercollidersubje = peterCollider.GetSubject();
	pettercollidersubje.AddObserver(petercmpt);
	auto& petterPeppersubje = petercmpt.GetSubject();

#pragma region Enemies
	auto& mrHotDogObj1 = sceneLevel1.CreateObject("mrHotDogObj1");
	auto& mrHotDogcmpt1 = mrHotDogObj1.AddComponent<MrHotDog>();
	auto& mrHotDogSpriteManager1 = mrHotDogObj1.AddComponent<SpriteManagerComponent>();
	mrHotDogSpriteManager1.AddSprite("Charachters/MrHotDog/MrHotDog_Idle.png", 1, 1, 0, 45, 45);
	mrHotDogSpriteManager1.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Left.png", 2, 1, 8, 45, 45);
	mrHotDogSpriteManager1.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Right.png", 2, 1, 8, 45, 45);
	mrHotDogSpriteManager1.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Up.png", 2, 1, 8, 45, 45);
	mrHotDogSpriteManager1.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Down.png", 2, 1, 8, 45, 45);
	mrHotDogSpriteManager1.AddSprite("Charachters/MrHotDog/MrHotDog_Death.png", 4, 1, 10, 45, 45);
	mrHotDogSpriteManager1.AddSprite("Charachters/MrHotDog/MrHotDog_Peppered.png", 2, 1, 8, 45, 45);
	mrHotDogcmpt1.SetDeathDuration(4 / 10.f);//4frames / 10fps

	auto& mrHotDogCollider1 = mrHotDogObj1.AddComponent<RectColliderComponent>();
	mrHotDogCollider1.Init({ 0,0,45,45 }, -2, true);
	mrHotDogObj1.AddComponent<MovementComponent>();
	auto& mrHotDogController1 = mrHotDogObj1.AddComponent<EnemyControllerComponent>();
	mrHotDogController1.SetPlayer1(&peterPepperObj);
	//mrHotDogObj1.SetLocalPosition(-18, 162);
	mrHotDogcmpt1.SetRespawnPosAndWalkDirection(-18, 162, EnemyControllerComponent::MovementState::Right);

	auto& mrHotDogcollidersubje = mrHotDogCollider1.GetSubject();
	mrHotDogcollidersubje.AddObserver(mrHotDogcmpt1);
	petterPeppersubje.AddObserver(mrHotDogcmpt1);
#pragma endregion

#pragma region InputCommandsPeter
	inputmanager.AddCommand<WalkRightCommand>(PCController::ControllerButton::Button_DPAD_RIGHT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkRightCommand>(InputManager::KeyboardKey::Key_D, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);

	inputmanager.AddCommand<WalkLeftCommand>(PCController::ControllerButton::Button_DPAD_LEFT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkLeftCommand>(InputManager::KeyboardKey::Key_A, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);

	inputmanager.AddCommand<WalkUpCommand>(PCController::ControllerButton::Button_DPAD_UP, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkUpCommand>(InputManager::KeyboardKey::Key_W, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);

	inputmanager.AddCommand<WalkDownCommand>(PCController::ControllerButton::Button_DPAD_DOWN, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkDownCommand>(InputManager::KeyboardKey::Key_S, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);

	auto& throwPepperCmdController1 = inputmanager.AddCommand<ThrowPepperCommand>(PCController::ControllerButton::Button_Square, InputManager::ButtonPressState::OnPressed);
	throwPepperCmdController1.SetPlayer(&peterPepperObj);
	throwPepperCmdController1.SetPepper(&pepperObj);
	auto& throwPepperCmdKeyBoard1 = inputmanager.AddCommand<ThrowPepperCommand>(InputManager::KeyboardKey::Key_CTRL_L, InputManager::ButtonPressState::OnPressed);
	throwPepperCmdKeyBoard1.SetPlayer(&peterPepperObj);
	throwPepperCmdKeyBoard1.SetPepper(&pepperObj);
#pragma endregion

#pragma region LadderSetup
	auto& ladderObj1 = sceneLevel1.CreateObject("ladderObj1");
	ladderObj1.SetLocalPosition(51.f, 342.f);
	ladderObj1.AddComponent<LadderComponent>();
	auto& ladder1colliderCmpt = ladderObj1.AddComponent<RectColliderComponent>();
	ladder1colliderCmpt.Init({ 0, 0, 3, 268 }, 10, true, { 255, 255, 0, 128 });

	auto& ladderObj2 = sceneLevel1.CreateObject("ladderObj2");
	ladderObj2.SetLocalPosition(51.f, 159.f);
	ladderObj2.AddComponent<LadderComponent>();
	auto& ladder2colliderCmpt = ladderObj2.AddComponent<RectColliderComponent>();
	ladder2colliderCmpt.Init({ 0, 0, 3, 137 }, 11, true, { 255, 255, 0, 128 });

	auto& ladderObj3 = sceneLevel1.CreateObject("ladderObj2");
	ladderObj3.SetLocalPosition(117.f, 251.f);
	ladderObj3.AddComponent<LadderComponent>();
	auto& ladder3colliderCmpt = ladderObj3.AddComponent<RectColliderComponent>();
	ladder3colliderCmpt.Init({ 0, 0, 4, 268 }, 12, true, { 255, 255, 0, 128 });

	auto& ladderObj4 = sceneLevel1.CreateObject("ladderObj4");
	ladderObj4.SetLocalPosition(184.f, 159.f);
	ladderObj4.AddComponent<LadderComponent>();
	auto& ladder4colliderCmpt = ladderObj4.AddComponent<RectColliderComponent>();
	ladder4colliderCmpt.Init({ 0, 0, 4, 450 }, 13, true, { 255, 255, 0, 128 });

	auto& ladderObj5 = sceneLevel1.CreateObject("ladderObj5");
	ladderObj5.SetLocalPosition(251.f, 159.f);
	ladderObj5.AddComponent<LadderComponent>();
	auto& ladder5colliderCmpt = ladderObj5.AddComponent<RectColliderComponent>();
	ladder5colliderCmpt.Init({ 0, 0, 4, 182 }, 14, true, { 255, 255, 0, 128 });

	auto& ladderObj6 = sceneLevel1.CreateObject("ladderObj6");
	ladderObj6.SetLocalPosition(318.f, 159.f);
	ladderObj6.AddComponent<LadderComponent>();
	auto& ladder6colliderCmpt = ladderObj6.AddComponent<RectColliderComponent>();
	ladder6colliderCmpt.Init({ 0, 0, 4, 450 }, 15, true, { 255, 255, 0, 128 });

	auto& ladderObj7 = sceneLevel1.CreateObject("ladderObj7");
	ladderObj7.SetLocalPosition(385.f, 252.f);
	ladderObj7.AddComponent<LadderComponent>();
	auto& ladder7colliderCmpt = ladderObj7.AddComponent<RectColliderComponent>();
	ladder7colliderCmpt.Init({ 0, 0, 4, 178 }, 16, true, { 255, 255, 0, 128 });

	auto& ladderObj8 = sceneLevel1.CreateObject("ladderObj8");
	ladderObj8.SetLocalPosition(452.f, 159.f);
	ladderObj8.AddComponent<LadderComponent>();
	auto& ladder8colliderCmpt = ladderObj8.AddComponent<RectColliderComponent>();
	ladder8colliderCmpt.Init({ 0, 0, 4, 450 }, 17, true, { 255, 255, 0, 128 });

	auto& ladderObj9 = sceneLevel1.CreateObject("ladderObj9");
	ladderObj9.SetLocalPosition(519.f, 340.f);
	ladderObj9.AddComponent<LadderComponent>();
	auto& ladder9colliderCmpt = ladderObj9.AddComponent<RectColliderComponent>();
	ladder9colliderCmpt.Init({ 0, 0, 3, 267 }, 18, true, { 255, 255, 0, 128 });

	auto& ladderObj10 = sceneLevel1.CreateObject("ladderObj10");
	ladderObj10.SetLocalPosition(586.f, 159.f);
	ladderObj10.AddComponent<LadderComponent>();
	auto& ladder10colliderCmpt = ladderObj10.AddComponent<RectColliderComponent>();
	ladder10colliderCmpt.Init({ 0, 0, 4, 227 }, 19, true, { 255, 255, 0, 128 });

	auto& ladderObj11 = sceneLevel1.CreateObject("ladderObj11");
	ladderObj11.SetLocalPosition(586.f, 429.f);
	ladderObj11.AddComponent<LadderComponent>();
	auto& ladder11colliderCmpt = ladderObj11.AddComponent<RectColliderComponent>();
	ladder11colliderCmpt.Init({ 0, 0, 4, 180 }, 20, true, { 255, 255, 0, 128 });
#pragma endregion

#pragma region WalkingPlatformSetup
	auto& walkPlatformObj1 = sceneLevel1.CreateObject("walkPlatformObj1");
	walkPlatformObj1.SetLocalPosition(30.f, 202.f);
	walkPlatformObj1.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform1colliderCmpt = walkPlatformObj1.AddComponent<RectColliderComponent>();
	walkPlatform1colliderCmpt.Init({ 0, 0, 580, 4 }, 50, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj2 = sceneLevel1.CreateObject("walkPlatformObj2");
	walkPlatformObj2.SetLocalPosition(30.f, 292.f);
	walkPlatformObj2.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform2colliderCmpt = walkPlatformObj2.AddComponent<RectColliderComponent>();
	walkPlatform2colliderCmpt.Init({ 0, 0, 175, 4 }, 51, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj3 = sceneLevel1.CreateObject("walkPlatformObj3");
	walkPlatformObj3.SetLocalPosition(300.f, 292.f);
	walkPlatformObj3.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform3colliderCmpt = walkPlatformObj3.AddComponent<RectColliderComponent>();
	walkPlatform3colliderCmpt.Init({ 0, 0, 310, 4 }, 52, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj4 = sceneLevel1.CreateObject("walkPlatformObj4");
	walkPlatformObj4.SetLocalPosition(165.f, 337.f);
	walkPlatformObj4.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform4colliderCmpt = walkPlatformObj4.AddComponent<RectColliderComponent>();
	walkPlatform4colliderCmpt.Init({ 0, 0, 176, 4 }, 53, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj5 = sceneLevel1.CreateObject("walkPlatformObj5");
	walkPlatformObj5.SetLocalPosition(30.f, 382.f);
	walkPlatformObj5.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform5colliderCmpt = walkPlatformObj5.AddComponent<RectColliderComponent>();
	walkPlatform5colliderCmpt.Init({ 0, 0, 178, 4 }, 54, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj6 = sceneLevel1.CreateObject("walkPlatformObj6");
	walkPlatformObj6.SetLocalPosition(432.f, 382.f);
	walkPlatformObj6.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform6colliderCmpt = walkPlatformObj6.AddComponent<RectColliderComponent>();
	walkPlatform6colliderCmpt.Init({ 0, 0, 178, 4 }, 55, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj7 = sceneLevel1.CreateObject("walkPlatformObj7");
	walkPlatformObj7.SetLocalPosition(165.f, 425.f);
	walkPlatformObj7.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform7colliderCmpt = walkPlatformObj7.AddComponent<RectColliderComponent>();
	walkPlatform7colliderCmpt.Init({ 0, 0, 310, 4 }, 56, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj8 = sceneLevel1.CreateObject("walkPlatformObj8");
	walkPlatformObj8.SetLocalPosition(433.f, 470.f);
	walkPlatformObj8.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform8colliderCmpt = walkPlatformObj8.AddComponent<RectColliderComponent>();
	walkPlatform8colliderCmpt.Init({ 0, 0, 178, 4 }, 57, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj9 = sceneLevel1.CreateObject("walkPlatformObj9");
	walkPlatformObj9.SetLocalPosition(30.f, 515.f);
	walkPlatformObj9.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform9colliderCmpt = walkPlatformObj9.AddComponent<RectColliderComponent>();
	walkPlatform9colliderCmpt.Init({ 0, 0, 445, 4 }, 58, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj10 = sceneLevel1.CreateObject("walkPlatformObj10");
	walkPlatformObj10.SetLocalPosition(30.f, 604.f);
	walkPlatformObj10.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform10colliderCmpt = walkPlatformObj10.AddComponent<RectColliderComponent>();
	walkPlatform10colliderCmpt.Init({ 0, 0, 580, 4 }, 59, true, { 0, 255, 0, 128 });
#pragma endregion

#pragma region IngredientsSetup
#pragma region burger1
	auto& bunBottomObj1 = sceneLevel1.CreateObject("bunBottomObj1");
	auto& bunBottomCmpt1 = bunBottomObj1.AddComponent<IngredientComponent>();
	auto& bunBottomSubje1 = bunBottomCmpt1.GetSubject();
	bunBottomObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider1 = bunBottomObj1.AddComponent<RectColliderComponent>();
	bunBottomcollider1.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunBottomObj1.SetLocalPosition(75.f, 595.f);
	auto& bunBottomColliderSubje1 = bunBottomcollider1.GetSubject();
	bunBottomColliderSubje1.AddObserver(bunBottomCmpt1);

	auto& burgerMeatObj1 = sceneLevel1.CreateObject("burgerMeatObj1");
	auto& burgerMeatCmpt1 = burgerMeatObj1.AddComponent<IngredientComponent>();
	auto& burgerMeatSubje1 = burgerMeatCmpt1.GetSubject();
	burgerMeatObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider1 = burgerMeatObj1.AddComponent<RectColliderComponent>();
	burgerMeatcollider1.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	burgerMeatObj1.SetLocalPosition(75.f, 505.f);
	auto& burgerMeatColliderSubje1 = burgerMeatcollider1.GetSubject();
	burgerMeatColliderSubje1.AddObserver(burgerMeatCmpt1);

	auto& lettuceObj1 = sceneLevel1.CreateObject("lettuceObj1");
	auto& lettuceCmpt1 = lettuceObj1.AddComponent<IngredientComponent>();
	auto& lettuceSubje1 = lettuceCmpt1.GetSubject();
	lettuceObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider1 = lettuceObj1.AddComponent<RectColliderComponent>();
	lettucecollider1.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	lettuceObj1.SetLocalPosition(75.f, 375.f);
	auto& lettuceColliderSubje1 = lettucecollider1.GetSubject();
	lettuceColliderSubje1.AddObserver(lettuceCmpt1);

	auto& bunTopObj1 = sceneLevel1.CreateObject("bunTopObj1");
	auto& bunTopCmpt1 = bunTopObj1.AddComponent<IngredientComponent>();
	auto& bunTopSubje1 = bunTopCmpt1.GetSubject();
	bunTopObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider1 = bunTopObj1.AddComponent<RectColliderComponent>();
	bunTopcollider1.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunTopObj1.SetLocalPosition(75.f, 285.f);
	auto& bunTopColliderSubje1 = bunTopcollider1.GetSubject();
	bunTopColliderSubje1.AddObserver(bunTopCmpt1);
#pragma endregion
#pragma region burger2
	auto& bunBottomObj2 = sceneLevel1.CreateObject("bunBottomObj2");
	auto& bunBottomCmpt2 = bunBottomObj2.AddComponent<IngredientComponent>();
	auto& bunBottomSubje2 = bunBottomCmpt2.GetSubject();
	bunBottomObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider2 = bunBottomObj2.AddComponent<RectColliderComponent>();
	bunBottomcollider2.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunBottomObj2.SetLocalPosition(209.f, 595.f);
	auto& bunBottomColliderSubje2 = bunBottomcollider2.GetSubject();
	bunBottomColliderSubje2.AddObserver(bunBottomCmpt2);

	auto& burgerMeatObj2 = sceneLevel1.CreateObject("burgerMeatObj2");
	auto& burgerMeatCmpt2 = burgerMeatObj2.AddComponent<IngredientComponent>();
	auto& burgerMeatSubje2 = burgerMeatCmpt2.GetSubject();
	burgerMeatObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider2 = burgerMeatObj2.AddComponent<RectColliderComponent>();
	burgerMeatcollider2.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	burgerMeatObj2.SetLocalPosition(209.f, 505.f);
	auto& burgerMeatColliderSubje2 = burgerMeatcollider2.GetSubject();
	burgerMeatColliderSubje2.AddObserver(burgerMeatCmpt2);

	auto& lettuceObj2 = sceneLevel1.CreateObject("lettuceObj2");
	auto& lettuceCmpt2 = lettuceObj2.AddComponent<IngredientComponent>();
	auto& lettuceSubje2 = lettuceCmpt2.GetSubject();
	lettuceObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider2 = lettuceObj2.AddComponent<RectColliderComponent>();
	lettucecollider2.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	lettuceObj2.SetLocalPosition(209.f, 415.f);
	auto& lettuceColliderSubje2 = lettucecollider2.GetSubject();
	lettuceColliderSubje2.AddObserver(lettuceCmpt2);

	auto& bunTopObj2 = sceneLevel1.CreateObject("bunTopObj2");
	auto& bunTopCmpt2 = bunTopObj2.AddComponent<IngredientComponent>();
	auto& bunTopSubje2 = bunTopCmpt2.GetSubject();
	bunTopObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider2 = bunTopObj2.AddComponent<RectColliderComponent>();
	bunTopcollider2.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunTopObj2.SetLocalPosition(209.f, 195.f);
	auto& bunTopColliderSubje2 = bunTopcollider2.GetSubject();
	bunTopColliderSubje2.AddObserver(bunTopCmpt2);
#pragma endregion
#pragma region burger3
	auto& bunBottomObj3 = sceneLevel1.CreateObject("bunBottomObj3");
	auto& bunBottomCmpt3 = bunBottomObj3.AddComponent<IngredientComponent>();
	auto& bunBottomSubje3 = bunBottomCmpt3.GetSubject();
	bunBottomObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider3 = bunBottomObj3.AddComponent<RectColliderComponent>();
	bunBottomcollider3.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunBottomObj3.SetLocalPosition(343.f, 595.f);
	auto& bunBottomColliderSubje3 = bunBottomcollider3.GetSubject();
	bunBottomColliderSubje3.AddObserver(bunBottomCmpt3);

	auto& burgerMeatObj3 = sceneLevel1.CreateObject("burgerMeatObj3");
	auto& burgerMeatCmpt3 = burgerMeatObj3.AddComponent<IngredientComponent>();
	auto& burgerMeatSubje3 = burgerMeatCmpt3.GetSubject();
	burgerMeatObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider3 = burgerMeatObj3.AddComponent<RectColliderComponent>();
	burgerMeatcollider3.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	burgerMeatObj3.SetLocalPosition(343.f, 415.f);
	auto& burgerMeatColliderSubje3 = burgerMeatcollider3.GetSubject();
	burgerMeatColliderSubje3.AddObserver(burgerMeatCmpt3);

	auto& lettuceObj3 = sceneLevel1.CreateObject("lettuceObj3");
	auto& lettuceCmpt3 = lettuceObj3.AddComponent<IngredientComponent>();
	auto& lettuceSubje3 = lettuceCmpt3.GetSubject();
	lettuceObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider3 = lettuceObj3.AddComponent<RectColliderComponent>();
	lettucecollider3.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	lettuceObj3.SetLocalPosition(343.f, 285.f);
	auto& lettuceColliderSubje3 = lettucecollider3.GetSubject();
	lettuceColliderSubje3.AddObserver(lettuceCmpt3);

	auto& bunTopObj3 = sceneLevel1.CreateObject("bunTopObj3");
	auto& bunTopCmpt3 = bunTopObj3.AddComponent<IngredientComponent>();
	auto& bunTopSubje3 = bunTopCmpt3.GetSubject();
	bunTopObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider3 = bunTopObj3.AddComponent<RectColliderComponent>();
	bunTopcollider3.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunTopObj3.SetLocalPosition(343.f, 195.f);
	auto& bunTopColliderSubje3 = bunTopcollider3.GetSubject();
	bunTopColliderSubje3.AddObserver(bunTopCmpt3);
#pragma endregion
#pragma region burger4
	auto& bunBottomObj4 = sceneLevel1.CreateObject("bunBottomObj4");
	auto& bunBottomCmpt4 = bunBottomObj4.AddComponent<IngredientComponent>();
	auto& bunBottomSubje4 = bunBottomCmpt4.GetSubject();
	bunBottomObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider4 = bunBottomObj4.AddComponent<RectColliderComponent>();
	bunBottomcollider4.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunBottomObj4.SetLocalPosition(477.f, 460.f);
	auto& bunBottomColliderSubje4 = bunBottomcollider4.GetSubject();
	bunBottomColliderSubje4.AddObserver(bunBottomCmpt4);

	auto& burgerMeatObj4 = sceneLevel1.CreateObject("burgerMeatObj4");
	auto& burgerMeatCmpt4 = burgerMeatObj4.AddComponent<IngredientComponent>();
	auto& burgerMeatSubje4 = burgerMeatCmpt4.GetSubject();
	burgerMeatObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider4 = burgerMeatObj4.AddComponent<RectColliderComponent>();
	burgerMeatcollider4.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	burgerMeatObj4.SetLocalPosition(477.f, 375.f);
	auto& burgerMeatColliderSubje4 = burgerMeatcollider4.GetSubject();
	burgerMeatColliderSubje4.AddObserver(burgerMeatCmpt4);

	auto& lettuceObj4 = sceneLevel1.CreateObject("lettuceObj4");
	auto& lettuceCmpt4 = lettuceObj4.AddComponent<IngredientComponent>();
	auto& lettuceSubje4 = lettuceCmpt4.GetSubject();
	lettuceObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider4 = lettuceObj4.AddComponent<RectColliderComponent>();
	lettucecollider4.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	lettuceObj4.SetLocalPosition(477.f, 285.f);
	auto& lettuceColliderSubje4 = lettucecollider4.GetSubject();
	lettuceColliderSubje4.AddObserver(lettuceCmpt4);

	auto& bunTopObj4 = sceneLevel1.CreateObject("bunTopObj4");
	auto& bunTopCmpt4 = bunTopObj4.AddComponent<IngredientComponent>();
	auto& bunTopSubje4 = bunTopCmpt4.GetSubject();
	bunTopObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider4 = bunTopObj4.AddComponent<RectColliderComponent>();
	bunTopcollider4.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunTopObj4.SetLocalPosition(477.f, 195.f);
	auto& bunTopColliderSubje4 = bunTopcollider4.GetSubject();
	bunTopColliderSubje4.AddObserver(bunTopCmpt4);
#pragma endregion
#pragma endregion

#pragma region PlateSetup
	auto& plateObj1 = sceneLevel1.CreateObject("plateObj1");
	auto& plateCmpt1 = plateObj1.AddComponent<PlateComponent>();
	plateCmpt1.SetIngredientFullCount(4);
	auto& plateSubje1 = plateCmpt1.GetSubject();
	plateObj1.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider1 = plateObj1.AddComponent<RectColliderComponent>();
	plateCollider1.Init({ 0, 8, 104, 4 }, 100, true, { 0, 133, 133, 255 });
	plateObj1.SetLocalPosition(68.f, 800.f);
	auto& plateColliderSubje1 = plateCollider1.GetSubject();
	plateColliderSubje1.AddObserver(plateCmpt1);

	auto& plateObj2 = sceneLevel1.CreateObject("plateObj2");
	auto& plateCmpt2 = plateObj2.AddComponent<PlateComponent>();
	plateCmpt2.SetIngredientFullCount(4);
	auto& plateSubje2 = plateCmpt2.GetSubject();
	plateObj2.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider2 = plateObj2.AddComponent<RectColliderComponent>();
	plateCollider2.Init({ 0, 8, 104, 4 }, 100, true, { 0, 133, 133, 255 });
	plateObj2.SetLocalPosition(202.f, 800.f);
	auto& plateColliderSubje2 = plateCollider2.GetSubject();
	plateColliderSubje2.AddObserver(plateCmpt2);

	auto& plateObj3 = sceneLevel1.CreateObject("plateObj3");
	auto& plateCmpt3 = plateObj3.AddComponent<PlateComponent>();
	plateCmpt3.SetIngredientFullCount(4);
	auto& plateSubje3 = plateCmpt3.GetSubject();
	plateObj3.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider3 = plateObj3.AddComponent<RectColliderComponent>();
	plateCollider3.Init({ 0, 8, 104, 4 }, 100, true, { 0, 133, 133, 255 });
	plateObj3.SetLocalPosition(336.f, 800.f);
	auto& plateColliderSubje3 = plateCollider3.GetSubject();
	plateColliderSubje3.AddObserver(plateCmpt3);

	auto& plateObj4 = sceneLevel1.CreateObject("plateObj4");
	auto& plateCmpt4 = plateObj4.AddComponent<PlateComponent>();
	plateCmpt4.SetIngredientFullCount(4);
	auto& plateSubje4 = plateCmpt4.GetSubject();
	plateObj4.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider4 = plateObj4.AddComponent<RectColliderComponent>();
	plateCollider4.Init({ 0, 8, 104, 4 }, 100, true, { 0, 133, 133, 255 });
	plateObj4.SetLocalPosition(470.f, 800.f);
	auto& plateColliderSubje4 = plateCollider4.GetSubject();
	plateColliderSubje4.AddObserver(plateCmpt4);
#pragma endregion

#pragma region UI/SCORE
	const int uiFontSize{ 20 };
	auto uiFont = ResourceManager::GetInstance().LoadFont("Fonts/BurgerTime_Font.otf", uiFontSize);

	//
	//Static UI
	//
#pragma region Static UI
	auto& oneUpTextObject = sceneLevel1.CreateObject("oneUpTextObject");
	oneUpTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 255, 0, 0 }, "1UP");
	oneUpTextObject.SetLocalPosition(float(uiFontSize * 4), 5);

	auto& hiScoreTextObject = sceneLevel1.CreateObject("hiScoreTextObject");
	hiScoreTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 255, 0, 0 }, "HI-SCORE");
	hiScoreTextObject.SetLocalPosition(float(uiFontSize * 4) + (uiFontSize * 5), 5);

	auto& pepperTextObject = sceneLevel1.CreateObject("pepperTextObject");
	pepperTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 0, 255, 0 }, "PEPPER");
	pepperTextObject.SetLocalPosition(float(windowW - (uiFontSize * 7)), 5);
#pragma endregion

	//
	//Dynamic UI
	//
	auto& peterLivesObj = sceneLevel1.CreateObject("peterLivesObj");
	auto& livescmpt = peterLivesObj.AddComponent<LivesComponent>();
	livescmpt.SetTexture("UI/Peter_Life_Icon.png", 3.25f, 3.25f);
	peterLivesObj.SetLocalPosition(10.f, windowH - 40.f);
	auto& livessubje = livescmpt.GetSubject();
	livessubje.AddObserver(gameManagercmpt);
	petterPeppersubje.AddObserver(livescmpt);


	auto& scoreObject = sceneLevel1.CreateObject("scoreObject");
	auto& scoretextcomp = scoreObject.AddComponent<TextComponent>();
	scoretextcomp.Setup(uiFont);
	auto& scoreCmpt = scoreObject.AddComponent<ScoreComponent>();
	scoreCmpt.SetTextComponent(scoretextcomp);
	scoreObject.SetLocalPosition(uiFontSize * 7.f, uiFontSize * 1.5f);
	auto& scoresubje = scoreCmpt.GetSubject();
	scoresubje.AddObserver(livescmpt);

#pragma region ScoreObservingIngredients
	bunBottomSubje1.AddObserver(scoreCmpt);
	bunBottomSubje2.AddObserver(scoreCmpt);
	bunBottomSubje3.AddObserver(scoreCmpt);
	bunBottomSubje4.AddObserver(scoreCmpt);
	burgerMeatSubje1.AddObserver(scoreCmpt);
	burgerMeatSubje2.AddObserver(scoreCmpt);
	burgerMeatSubje3.AddObserver(scoreCmpt);
	burgerMeatSubje4.AddObserver(scoreCmpt);
	lettuceSubje1.AddObserver(scoreCmpt);
	lettuceSubje2.AddObserver(scoreCmpt);
	lettuceSubje3.AddObserver(scoreCmpt);
	lettuceSubje4.AddObserver(scoreCmpt);
	bunTopSubje1.AddObserver(scoreCmpt);
	bunTopSubje2.AddObserver(scoreCmpt);
	bunTopSubje3.AddObserver(scoreCmpt);
	bunTopSubje4.AddObserver(scoreCmpt);
#pragma endregion




	//TODO: hi score component should read hi-score from file and observe the score object,
	//if the score components value is bigger than the current high score update it

	auto& hiScoreObject = sceneLevel1.CreateObject("scoreObject");
	auto& hiScoretextcomp = hiScoreObject.AddComponent<TextComponent>();
	hiScoretextcomp.Setup(uiFont);
	auto& hiScoreCmpt = hiScoreObject.AddComponent<HiScoreComponent>();
	hiScoreCmpt.SetTextComponent(hiScoretextcomp);
	hiScoreCmpt.SetScoreComponent(scoreCmpt);
	hiScoreObject.SetLocalPosition(uiFontSize * 15.f, uiFontSize * 1.5f);

	auto& pepperCountObject = sceneLevel1.CreateObject("pepperCountObject");
	auto& pepperCountComp = pepperCountObject.AddComponent<TextComponent>();
	pepperCountComp.Setup(uiFont);
	pepperCountObject.AddComponent<PepperCountComponent>().SetTextComponent(pepperCountComp);
	pepperCountObject.SetLocalPosition(windowW - (uiFontSize * 2.f), uiFontSize * 1.5f);
#pragma endregion


	auto& gameManagersubje = gameManagercmpt.GetSubject();
	gameManagersubje.AddObserver(petercmpt);
	if (m_GameMode == GameMode::Coop)
	{
		//add extra player
	}

	gameManagersubje.AddObserver(mrHotDogcmpt1);
	//gameManagersubje.AddObserver(mrHotDogcmpt2);
	//gameManagersubje.AddObserver(mrHotDogcmpt3);
	//gameManagersubje.AddObserver(mrEggcmpt1);

	plateSubje1.AddObserver(gameManagercmpt);
	plateSubje2.AddObserver(gameManagercmpt);
	plateSubje3.AddObserver(gameManagercmpt);
	plateSubje4.AddObserver(gameManagercmpt);

#pragma endregion

}

void dae::MainMenuComponent::LoadLevel2()
{
	auto& sceneLevel2 = SceneManager::GetInstance().CreateScene("Level2");
	//auto& inputmanager = InputManager::GetInstance();

	auto fpsFont = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 19);

	int windowW{}, windowH{};
	SDL_GetWindowSize(m_pEngine->GetWindow(), &windowW, &windowH);

	auto& backgroundObject = sceneLevel2.CreateObject("backgroundObject");
	backgroundObject.AddComponent<Texture2DComponent>().SetTexture("Level/Level1.png");

	auto& fpsCounterObj = sceneLevel2.CreateObject("fpsCounterObj");
	auto& fpstextComponent = fpsCounterObj.AddComponent<TextComponent>();
	auto& fpsComponent = fpsCounterObj.AddComponent<FPSComponent>();
	fpsComponent.SetTextComponent(fpstextComponent);
	fpsCounterObj.SetLocalPosition(5.f, 5.f);
	fpstextComponent.SetFont(fpsFont);
	fpstextComponent.SetTextColor({ 255, 255, 0 });
}

void dae::MainMenuComponent::LoadLevel3()
{
	auto& sceneLevel3 = SceneManager::GetInstance().CreateScene("Level3");
	//auto& inputmanager = InputManager::GetInstance();

	auto fpsFont = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 19);

	int windowW{}, windowH{};
	SDL_GetWindowSize(m_pEngine->GetWindow(), &windowW, &windowH);

	auto& backgroundObject = sceneLevel3.CreateObject("backgroundObject");
	backgroundObject.AddComponent<Texture2DComponent>().SetTexture("Level/Level1.png");

	auto& fpsCounterObj = sceneLevel3.CreateObject("fpsCounterObj");
	auto& fpstextComponent = fpsCounterObj.AddComponent<TextComponent>();
	auto& fpsComponent = fpsCounterObj.AddComponent<FPSComponent>();
	fpsComponent.SetTextComponent(fpstextComponent);
	fpsCounterObj.SetLocalPosition(5.f, 5.f);
	fpstextComponent.SetFont(fpsFont);
	fpstextComponent.SetTextColor({ 255, 0, 0 });
}
