#include "BurgerTimePCH.h"
#include "MainMenuComponent.h"

#include <NightOwlEngine.h>

#pragma region Components importing
#include <InputManager.h>
#include <SceneManager.h>
#include <Renderer.h>
#include <ResourceManager.h>

#include <Scene.h>
#include <GameObject.h>

#include <Texture2DComponent.h>
#include <TextComponent.h>
#include <FPSComponent.h>

#include <Subject.h>
#include <SpriteComponent.h>
#include <RectColliderComponent.h>
#include <SpriteManagerComponent.h>

#include "MovementComponent.h"
#include "PeterPepper.h"
#include "MrHotDog.h"
#include "MrEgg.h"
#include "MrPickle.h"
#include "EnemyControllerComponent.h"

#include "LevelManager.h"

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

#include "BurgerTimeCommands.h"
#include "ServiceLocator.h"
#include "SDLSoundSystem.h"

dae::MainMenuComponent::MainMenuComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_pEngine{ nullptr }
	, m_SelectedButtonIndex{ 0 }
	, m_GameMode{ GameMode::MainMenu }
	, m_ScenesLoadedBefore{ false }
	, m_CoopLoadedBefore{ false }
	, m_VersusLoadedBefore{ false }
{
}

void dae::MainMenuComponent::SelectNextButton()
{
	if (m_GameMode != GameMode::MainMenu)
		return;

	m_pButtons[m_SelectedButtonIndex]->SetSelectedState(false);

	++m_SelectedButtonIndex;
	if (m_SelectedButtonIndex >= static_cast<int>(m_pButtons.size()))
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
	if (!m_ScenesLoadedBefore)
	{
		if (m_GameMode == GameMode::Coop || m_GameMode == GameMode::Versus)
		{
			//this shouldn't be needed and it should just automatically add/remove controllers
			InputManager::GetInstance().AddController();
		}

		LoadLevel1();
		LoadLevel2();
		LoadLevel3();
	}

	for (auto& pLevelManager : m_pLevelManagers)
	{
		pLevelManager->SetGameMode(m_GameMode);
	}

	//m_pLevelManagers;

	/*
	//SetGameMode
	//SetEnemyPlayer
	//SetCoopPlayer
	AddEnemy
	SetPepperObject
	*/

	//once all scenes have been loaded in start scene of level 1
	auto& scene = SceneManager::GetInstance();
	scene.GotoNextScene();
	m_ScenesLoadedBefore = true;//so that they objects aren't added multiple times to their respected scene
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

	auto& backgroundObject = sceneLevel1.CreateObject("backgroundObject");
	backgroundObject.AddComponent<Texture2DComponent>().SetTexture("Level/Level1.png");

	auto& fpsCounterObj = sceneLevel1.CreateObject("fpsCounterObj");
	auto& fpstextComponent = fpsCounterObj.AddComponent<TextComponent>();
	auto& fpsComponent = fpsCounterObj.AddComponent<FPSComponent>();
	fpsComponent.SetTextComponent(fpstextComponent);
	fpsCounterObj.SetLocalPosition(5.f, 5.f);
	fpstextComponent.SetFont(fpsFont);
	fpstextComponent.SetTextColor({ 0, 255, 255 });

	auto& levelManagerObj = sceneLevel1.CreateObject("Obj");
	auto& levelManagercmpt = levelManagerObj.AddComponent<LevelManager>();
	levelManagercmpt.SetMaxFullPlateCount(4);
	levelManagercmpt.SetLastSceneIndex(3);//there are only 3 levels in the game

	//register the level manager so that it gets updated when the gamemodechanges
	m_pLevelManagers.emplace_back(&levelManagercmpt);

#pragma region Pepper_PeterAndSally
	//pepper object to throw on enemies
	auto& pepperObj = sceneLevel1.CreateObject("pepperObj");
	auto& pepperCmpt = pepperObj.AddComponent<PepperComponent>();
	auto& pepperSpriteManager = pepperObj.AddComponent<SpriteManagerComponent>();
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Left.png", 4, 1, 4, 45, 45);
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Right.png", 4, 1, 4, 45, 45);
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Up.png", 4, 1, 4, 45, 45);
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Down.png", 4, 1, 4, 45, 45);
	auto& pepperCollider = pepperObj.AddComponent<RectColliderComponent>();
	pepperCollider.Init({ 0,0,45,45 }, 1, true);
	pepperObj.SetLocalPosition(-100.f, -100.f);
	pepperCmpt.SetResetPos(-100.f, -100.f);
	pepperCmpt.SetSpriteDuration(1.f);

	auto& peppercollidersubje = pepperCollider.GetSubject();
	peppercollidersubje.AddObserver(pepperCmpt);

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
	peterCollider.Init({ 0,1,45,44 }, 1, true);
	peterPepperObj.AddComponent<MovementComponent>();
	petercmpt.SetSpawnLocation(298.f, 563.f);
	peterPepperObj.SetLocalPosition(298.f, 563.f);

	auto& pettercollidersubje = peterCollider.GetSubject();
	pettercollidersubje.AddObserver(petercmpt);
	auto& petterPeppersubje = petercmpt.GetSubject();


	//sally salt
	auto& sallySaltObj = sceneLevel1.CreateObject("sallySaltObj");
	auto& sallycmpt = sallySaltObj.AddComponent<PeterPepper>();
	auto& sallySpriteManager = sallySaltObj.AddComponent<SpriteManagerComponent>();
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Idle.png", 1, 1, 0, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Walking_Left.png", 4, 1, 16, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Walking_Right.png", 4, 1, 16, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Walking_Up.png", 4, 1, 16, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Walking_Down.png", 4, 1, 16, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Death.png", 6, 1, 2, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Won.png", 2, 1, 2, 45, 45);

	auto& sallyCollider = sallySaltObj.AddComponent<RectColliderComponent>();
	sallyCollider.Init({ 0,1,45,44 }, 1, true);
	sallySaltObj.AddComponent<MovementComponent>();
	sallycmpt.SetSpawnLocation(298.f, 384.f);
	sallySaltObj.SetLocalPosition(298.f, 384.f);

	auto& sallyColliderSubje = sallyCollider.GetSubject();
	sallyColliderSubje.AddObserver(sallycmpt);
	auto& sallySaltSubje = sallycmpt.GetSubject();

	//if not in coop mode to begin with then disable the coop player object then 
	if (m_GameMode != GameMode::Coop)
		sallySaltObj.SetEnabledState(false);
	else
		sallySaltObj.SetEnabledState(true);

	levelManagercmpt.SetNormalPlayer(&peterPepperObj);
	levelManagercmpt.SetCoopPlayer(&sallySaltObj);
	levelManagercmpt.SetPepperObject(&pepperObj);
#pragma endregion

#pragma region Enemies
#pragma region EnemyPlayer
	auto& playerHotDogObj = sceneLevel1.CreateObject("playerHotDogObj");
	auto& playerHotDogCmpt = playerHotDogObj.AddComponent<MrHotDog>();
	playerHotDogCmpt.SetControlledByHuman(true);
	auto& playerHotDogSpriteManager = playerHotDogObj.AddComponent<SpriteManagerComponent>();
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Idle.png", 1, 1, 0, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Left.png", 2, 1, 8, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Right.png", 2, 1, 8, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Up.png", 2, 1, 8, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Down.png", 2, 1, 8, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Death.png", 4, 1, 10, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Peppered.png", 2, 1, 8, 45, 45);
	playerHotDogCmpt.SetDeathDuration(4 / 10.f);//4frames / 10fps

	auto& playerHotDogCollider = playerHotDogObj.AddComponent<RectColliderComponent>();
	playerHotDogCollider.Init({ 0,1,45,44 }, 1, true);
	playerHotDogObj.AddComponent<MovementComponent>();
	playerHotDogCmpt.SetRespawnPosAndWalkDirection(30, 162, EnemyControllerComponent::MovementState::Right);
	auto& playerHotDogcollidersubje = playerHotDogCollider.GetSubject();
	playerHotDogcollidersubje.AddObserver(playerHotDogCmpt);
#pragma endregion

#pragma region Enemy1
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
	mrHotDogCollider1.Init({ 0,1,45,44 }, 1, true);
	mrHotDogObj1.AddComponent<MovementComponent>();
	auto& mrHotDogController1 = mrHotDogObj1.AddComponent<EnemyControllerComponent>();

	mrHotDogcmpt1.SetRespawnPosAndWalkDirection(30, 162, EnemyControllerComponent::MovementState::Right);
	auto& mrHotDogcollidersubje = mrHotDogCollider1.GetSubject();
	mrHotDogcollidersubje.AddObserver(mrHotDogcmpt1);
#pragma endregion

	switch (m_GameMode)
	{
	case dae::GameMode::Coop://set enemy player and player 2, then let it fall through to add player 1

		mrHotDogController1.SetPlayer2(&sallySaltObj);
		//mrHotDogController2.SetPlayer2(&sallySaltObj);
		//mrHotDogController3.SetPlayer2(&sallySaltObj);
		//mrEggController1.SetPlayer2(&sallySaltObj);

	case dae::GameMode::SinglePlayer:
		playerHotDogObj.SetEnabledState(false);
		//mrHotDogController2.SetPlayer1(&peterPepperObj);
		//mrHotDogController2.SetPlayer1(&peterPepperObj);
		//mrEggController1.SetPlayer1(&peterPepperObj);

		break;
	case dae::GameMode::Versus: //if in versus mode to begin with then disable the normal enemie objects
		playerHotDogObj.SetEnabledState(true);

		mrHotDogObj1.SetEnabledState(false);
		//mrHotDogObj2.SetEnabledState(false);
		//mrHotDogObj3.SetEnabledState(false);
		//mrEggObj1.SetEnabledState(false);
		break;
	}

	mrHotDogController1.SetPlayer1(&peterPepperObj);

	//register enemy player to the levelmanager
	levelManagercmpt.SetEnemyPlayer(&playerHotDogObj);

	//register enemies to the levelmanager
	levelManagercmpt.AddEnemy(&mrHotDogObj1);
	//levelManagercmpt.AddEnemy(&mrHotDogObj2);
	//levelManagercmpt.AddEnemy(&mrHotDogObj3);
	//levelManagercmpt.AddEnemy(&mrEggObj1);


	//let the enemies observe the player(s)
	petterPeppersubje.AddObserver(playerHotDogCmpt);

	petterPeppersubje.AddObserver(mrHotDogcmpt1);
	//petterPeppersubje.AddObserver(mrHotDogcmpt2);
	//petterPeppersubje.AddObserver(mrHotDogcmpt3);
	//petterPeppersubje.AddObserver(mrEggcmpt1);

	sallySaltSubje.AddObserver(playerHotDogCmpt);

	sallySaltSubje.AddObserver(mrHotDogcmpt1);
	//sallySaltSubje.AddObserver(mrHotDogcmpt2);
	//sallySaltSubje.AddObserver(mrHotDogcmpt3);
	//sallySaltSubje.AddObserver(mrEggcmpt1);
#pragma endregion

#pragma region LadderSetup
	auto& ladderObj1 = sceneLevel1.CreateObject("ladderObj1");
	ladderObj1.SetLocalPosition(51.f, 372.f);
	ladderObj1.AddComponent<LadderComponent>();
	auto& ladder1colliderCmpt = ladderObj1.AddComponent<RectColliderComponent>();
	ladder1colliderCmpt.Init({ 0, 0, 3, 238 }, 1, true, { 255, 255, 0, 128 });

	auto& ladderObj2 = sceneLevel1.CreateObject("ladderObj2");
	ladderObj2.SetLocalPosition(51.f, 189.f);
	ladderObj2.AddComponent<LadderComponent>();
	auto& ladder2colliderCmpt = ladderObj2.AddComponent<RectColliderComponent>();
	ladder2colliderCmpt.Init({ 0, 0, 3, 107 }, 1, true, { 255, 255, 0, 128 });

	auto& ladderObj3 = sceneLevel1.CreateObject("ladderObj3");
	ladderObj3.SetLocalPosition(117.f, 281.f);
	ladderObj3.AddComponent<LadderComponent>();
	auto& ladder3colliderCmpt = ladderObj3.AddComponent<RectColliderComponent>();
	ladder3colliderCmpt.Init({ 0, 0, 4, 238 }, 1, true, { 255, 255, 0, 128 });

	auto& ladderObj4 = sceneLevel1.CreateObject("ladderObj4");
	ladderObj4.SetLocalPosition(184.f, 189.f);
	ladderObj4.AddComponent<LadderComponent>();
	auto& ladder4colliderCmpt = ladderObj4.AddComponent<RectColliderComponent>();
	ladder4colliderCmpt.Init({ 0, 0, 4, 420 }, 1, true, { 255, 255, 0, 128 });

	auto& ladderObj5 = sceneLevel1.CreateObject("ladderObj5");
	ladderObj5.SetLocalPosition(251.f, 189.f);
	ladderObj5.AddComponent<LadderComponent>();
	auto& ladder5colliderCmpt = ladderObj5.AddComponent<RectColliderComponent>();
	ladder5colliderCmpt.Init({ 0, 0, 4, 152 }, 1, true, { 255, 255, 0, 128 });

	auto& ladderObj6 = sceneLevel1.CreateObject("ladderObj6");
	ladderObj6.SetLocalPosition(318.f, 189.f);
	ladderObj6.AddComponent<LadderComponent>();
	auto& ladder6colliderCmpt = ladderObj6.AddComponent<RectColliderComponent>();
	ladder6colliderCmpt.Init({ 0, 0, 4, 420 }, 1, true, { 255, 255, 0, 128 });

	auto& ladderObj7 = sceneLevel1.CreateObject("ladderObj7");
	ladderObj7.SetLocalPosition(385.f, 282.f);
	ladderObj7.AddComponent<LadderComponent>();
	auto& ladder7colliderCmpt = ladderObj7.AddComponent<RectColliderComponent>();
	ladder7colliderCmpt.Init({ 0, 0, 4, 148 }, 1, true, { 255, 255, 0, 128 });

	auto& ladderObj8 = sceneLevel1.CreateObject("ladderObj8");
	ladderObj8.SetLocalPosition(452.f, 189.f);
	ladderObj8.AddComponent<LadderComponent>();
	auto& ladder8colliderCmpt = ladderObj8.AddComponent<RectColliderComponent>();
	ladder8colliderCmpt.Init({ 0, 0, 4, 420 }, 1, true, { 255, 255, 0, 128 });

	auto& ladderObj9 = sceneLevel1.CreateObject("ladderObj9");
	ladderObj9.SetLocalPosition(519.f, 370.f);
	ladderObj9.AddComponent<LadderComponent>();
	auto& ladder9colliderCmpt = ladderObj9.AddComponent<RectColliderComponent>();
	ladder9colliderCmpt.Init({ 0, 0, 3, 237 }, 1, true, { 255, 255, 0, 128 });

	auto& ladderObj10 = sceneLevel1.CreateObject("ladderObj10");
	ladderObj10.SetLocalPosition(586.f, 189.f);
	ladderObj10.AddComponent<LadderComponent>();
	auto& ladder10colliderCmpt = ladderObj10.AddComponent<RectColliderComponent>();
	ladder10colliderCmpt.Init({ 0, 0, 4, 197 }, 1, true, { 255, 255, 0, 128 });

	auto& ladderObj11 = sceneLevel1.CreateObject("ladderObj11");
	ladderObj11.SetLocalPosition(586.f, 459.f);
	ladderObj11.AddComponent<LadderComponent>();
	auto& ladder11colliderCmpt = ladderObj11.AddComponent<RectColliderComponent>();
	ladder11colliderCmpt.Init({ 0, 0, 4, 150 }, 1, true, { 255, 255, 0, 128 });
#pragma endregion

#pragma region WalkingPlatformSetup
	auto& walkPlatformObj1 = sceneLevel1.CreateObject("walkPlatformObj1");
	walkPlatformObj1.SetLocalPosition(30.f, 202.f);
	walkPlatformObj1.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform1colliderCmpt = walkPlatformObj1.AddComponent<RectColliderComponent>();
	walkPlatform1colliderCmpt.Init({ 0, 0, 580, 4 }, 1, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj2 = sceneLevel1.CreateObject("walkPlatformObj2");
	walkPlatformObj2.SetLocalPosition(30.f, 292.f);
	walkPlatformObj2.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform2colliderCmpt = walkPlatformObj2.AddComponent<RectColliderComponent>();
	walkPlatform2colliderCmpt.Init({ 0, 0, 175, 4 }, 1, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj3 = sceneLevel1.CreateObject("walkPlatformObj3");
	walkPlatformObj3.SetLocalPosition(300.f, 292.f);
	walkPlatformObj3.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform3colliderCmpt = walkPlatformObj3.AddComponent<RectColliderComponent>();
	walkPlatform3colliderCmpt.Init({ 0, 0, 310, 4 }, 1, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj4 = sceneLevel1.CreateObject("walkPlatformObj4");
	walkPlatformObj4.SetLocalPosition(165.f, 337.f);
	walkPlatformObj4.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform4colliderCmpt = walkPlatformObj4.AddComponent<RectColliderComponent>();
	walkPlatform4colliderCmpt.Init({ 0, 0, 176, 4 }, 1, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj5 = sceneLevel1.CreateObject("walkPlatformObj5");
	walkPlatformObj5.SetLocalPosition(30.f, 382.f);
	walkPlatformObj5.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform5colliderCmpt = walkPlatformObj5.AddComponent<RectColliderComponent>();
	walkPlatform5colliderCmpt.Init({ 0, 0, 178, 4 }, 1, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj6 = sceneLevel1.CreateObject("walkPlatformObj6");
	walkPlatformObj6.SetLocalPosition(432.f, 382.f);
	walkPlatformObj6.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform6colliderCmpt = walkPlatformObj6.AddComponent<RectColliderComponent>();
	walkPlatform6colliderCmpt.Init({ 0, 0, 178, 4 }, 1, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj7 = sceneLevel1.CreateObject("walkPlatformObj7");
	walkPlatformObj7.SetLocalPosition(165.f, 425.f);
	walkPlatformObj7.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform7colliderCmpt = walkPlatformObj7.AddComponent<RectColliderComponent>();
	walkPlatform7colliderCmpt.Init({ 0, 0, 310, 4 }, 1, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj8 = sceneLevel1.CreateObject("walkPlatformObj8");
	walkPlatformObj8.SetLocalPosition(433.f, 470.f);
	walkPlatformObj8.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform8colliderCmpt = walkPlatformObj8.AddComponent<RectColliderComponent>();
	walkPlatform8colliderCmpt.Init({ 0, 0, 178, 4 }, 1, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj9 = sceneLevel1.CreateObject("walkPlatformObj9");
	walkPlatformObj9.SetLocalPosition(30.f, 515.f);
	walkPlatformObj9.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform9colliderCmpt = walkPlatformObj9.AddComponent<RectColliderComponent>();
	walkPlatform9colliderCmpt.Init({ 0, 0, 445, 4 }, 1, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj10 = sceneLevel1.CreateObject("walkPlatformObj10");
	walkPlatformObj10.SetLocalPosition(30.f, 604.f);
	walkPlatformObj10.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform10colliderCmpt = walkPlatformObj10.AddComponent<RectColliderComponent>();
	walkPlatform10colliderCmpt.Init({ 0, 0, 580, 4 }, 1, true, { 0, 255, 0, 128 });
#pragma endregion

#pragma region IngredientsSetup
#pragma region burger1
	auto& bunTopObj1 = sceneLevel1.CreateObject("bunTopObj1");
	auto& bunTopCmpt1 = bunTopObj1.AddComponent<IngredientComponent>();
	auto& bunTopSubje1 = bunTopCmpt1.GetSubject();
	bunTopObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider1 = bunTopObj1.AddComponent<RectColliderComponent>();
	bunTopcollider1.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	bunTopObj1.SetLocalPosition(75.f, 280.f);
	bunTopCmpt1.SetSpawnLocation(75.f, 280.f);
	auto& bunTopColliderSubje1 = bunTopcollider1.GetSubject();
	bunTopColliderSubje1.AddObserver(bunTopCmpt1);

	auto& lettuceObj1 = sceneLevel1.CreateObject("lettuceObj1");
	auto& lettuceCmpt1 = lettuceObj1.AddComponent<IngredientComponent>();
	auto& lettuceSubje1 = lettuceCmpt1.GetSubject();
	lettuceObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider1 = lettuceObj1.AddComponent<RectColliderComponent>();
	lettucecollider1.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	lettuceObj1.SetLocalPosition(75.f, 370.f);
	lettuceCmpt1.SetSpawnLocation(75.f, 370.f);
	auto& lettuceColliderSubje1 = lettucecollider1.GetSubject();
	lettuceColliderSubje1.AddObserver(lettuceCmpt1);

	auto& burgerMeatObj1 = sceneLevel1.CreateObject("burgerMeatObj1");
	auto& burgerMeatCmpt1 = burgerMeatObj1.AddComponent<IngredientComponent>();
	auto& burgerMeatSubje1 = burgerMeatCmpt1.GetSubject();
	burgerMeatObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider1 = burgerMeatObj1.AddComponent<RectColliderComponent>();
	burgerMeatcollider1.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	burgerMeatObj1.SetLocalPosition(75.f, 500.f);
	burgerMeatCmpt1.SetSpawnLocation(75.f, 500.f);
	auto& burgerMeatColliderSubje1 = burgerMeatcollider1.GetSubject();
	burgerMeatColliderSubje1.AddObserver(burgerMeatCmpt1);

	auto& bunBottomObj1 = sceneLevel1.CreateObject("bunBottomObj1");
	auto& bunBottomCmpt1 = bunBottomObj1.AddComponent<IngredientComponent>();
	auto& bunBottomSubje1 = bunBottomCmpt1.GetSubject();
	bunBottomObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider1 = bunBottomObj1.AddComponent<RectColliderComponent>();
	bunBottomcollider1.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	bunBottomObj1.SetLocalPosition(75.f, 590.f);
	bunBottomCmpt1.SetSpawnLocation(75.f, 590.f);
	auto& bunBottomColliderSubje1 = bunBottomcollider1.GetSubject();
	bunBottomColliderSubje1.AddObserver(bunBottomCmpt1);
#pragma endregion
#pragma region burger2
	auto& bunTopObj2 = sceneLevel1.CreateObject("bunTopObj2");
	auto& bunTopCmpt2 = bunTopObj2.AddComponent<IngredientComponent>();
	auto& bunTopSubje2 = bunTopCmpt2.GetSubject();
	bunTopObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider2 = bunTopObj2.AddComponent<RectColliderComponent>();
	bunTopcollider2.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	bunTopObj2.SetLocalPosition(209.f, 190.f);
	bunTopCmpt2.SetSpawnLocation(209.f, 190.f);
	auto& bunTopColliderSubje2 = bunTopcollider2.GetSubject();
	bunTopColliderSubje2.AddObserver(bunTopCmpt2);

	auto& lettuceObj2 = sceneLevel1.CreateObject("lettuceObj2");
	auto& lettuceCmpt2 = lettuceObj2.AddComponent<IngredientComponent>();
	auto& lettuceSubje2 = lettuceCmpt2.GetSubject();
	lettuceObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider2 = lettuceObj2.AddComponent<RectColliderComponent>();
	lettucecollider2.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	lettuceObj2.SetLocalPosition(209.f, 410.f);
	lettuceCmpt2.SetSpawnLocation(209.f, 410.f);
	auto& lettuceColliderSubje2 = lettucecollider2.GetSubject();
	lettuceColliderSubje2.AddObserver(lettuceCmpt2);

	auto& burgerMeatObj2 = sceneLevel1.CreateObject("burgerMeatObj2");
	auto& burgerMeatCmpt2 = burgerMeatObj2.AddComponent<IngredientComponent>();
	auto& burgerMeatSubje2 = burgerMeatCmpt2.GetSubject();
	burgerMeatObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider2 = burgerMeatObj2.AddComponent<RectColliderComponent>();
	burgerMeatcollider2.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	burgerMeatObj2.SetLocalPosition(209.f, 500.f);
	burgerMeatCmpt2.SetSpawnLocation(209.f, 500.f);
	auto& burgerMeatColliderSubje2 = burgerMeatcollider2.GetSubject();
	burgerMeatColliderSubje2.AddObserver(burgerMeatCmpt2);

	auto& bunBottomObj2 = sceneLevel1.CreateObject("bunBottomObj2");
	auto& bunBottomCmpt2 = bunBottomObj2.AddComponent<IngredientComponent>();
	auto& bunBottomSubje2 = bunBottomCmpt2.GetSubject();
	bunBottomObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider2 = bunBottomObj2.AddComponent<RectColliderComponent>();
	bunBottomcollider2.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	bunBottomObj2.SetLocalPosition(209.f, 590.f);
	bunBottomCmpt2.SetSpawnLocation(209.f, 590.f);
	auto& bunBottomColliderSubje2 = bunBottomcollider2.GetSubject();
	bunBottomColliderSubje2.AddObserver(bunBottomCmpt2);
#pragma endregion
#pragma region burger3
	auto& bunTopObj3 = sceneLevel1.CreateObject("bunTopObj3");
	auto& bunTopCmpt3 = bunTopObj3.AddComponent<IngredientComponent>();
	auto& bunTopSubje3 = bunTopCmpt3.GetSubject();
	bunTopObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider3 = bunTopObj3.AddComponent<RectColliderComponent>();
	bunTopcollider3.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	bunTopObj3.SetLocalPosition(343.f, 190.f);
	bunTopCmpt3.SetSpawnLocation(343.f, 190.f);
	auto& bunTopColliderSubje3 = bunTopcollider3.GetSubject();
	bunTopColliderSubje3.AddObserver(bunTopCmpt3);

	auto& lettuceObj3 = sceneLevel1.CreateObject("lettuceObj3");
	auto& lettuceCmpt3 = lettuceObj3.AddComponent<IngredientComponent>();
	auto& lettuceSubje3 = lettuceCmpt3.GetSubject();
	lettuceObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider3 = lettuceObj3.AddComponent<RectColliderComponent>();
	lettucecollider3.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	lettuceObj3.SetLocalPosition(343.f, 280.f);
	lettuceCmpt3.SetSpawnLocation(343.f, 280.f);
	auto& lettuceColliderSubje3 = lettucecollider3.GetSubject();
	lettuceColliderSubje3.AddObserver(lettuceCmpt3);

	auto& burgerMeatObj3 = sceneLevel1.CreateObject("burgerMeatObj3");
	auto& burgerMeatCmpt3 = burgerMeatObj3.AddComponent<IngredientComponent>();
	auto& burgerMeatSubje3 = burgerMeatCmpt3.GetSubject();
	burgerMeatObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider3 = burgerMeatObj3.AddComponent<RectColliderComponent>();
	burgerMeatcollider3.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	burgerMeatObj3.SetLocalPosition(343.f, 410.f);
	burgerMeatCmpt3.SetSpawnLocation(343.f, 410.f);
	auto& burgerMeatColliderSubje3 = burgerMeatcollider3.GetSubject();
	burgerMeatColliderSubje3.AddObserver(burgerMeatCmpt3);

	auto& bunBottomObj3 = sceneLevel1.CreateObject("bunBottomObj3");
	auto& bunBottomCmpt3 = bunBottomObj3.AddComponent<IngredientComponent>();
	auto& bunBottomSubje3 = bunBottomCmpt3.GetSubject();
	bunBottomObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider3 = bunBottomObj3.AddComponent<RectColliderComponent>();
	bunBottomcollider3.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	bunBottomObj3.SetLocalPosition(343.f, 590.f);
	bunBottomCmpt3.SetSpawnLocation(343.f, 590.f);
	auto& bunBottomColliderSubje3 = bunBottomcollider3.GetSubject();
	bunBottomColliderSubje3.AddObserver(bunBottomCmpt3);
#pragma endregion
#pragma region burger4
	auto& bunTopObj4 = sceneLevel1.CreateObject("bunTopObj4");
	auto& bunTopCmpt4 = bunTopObj4.AddComponent<IngredientComponent>();
	auto& bunTopSubje4 = bunTopCmpt4.GetSubject();
	bunTopObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider4 = bunTopObj4.AddComponent<RectColliderComponent>();
	bunTopcollider4.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	bunTopObj4.SetLocalPosition(477.f, 190.f);
	bunTopCmpt4.SetSpawnLocation(477.f, 190.f);
	auto& bunTopColliderSubje4 = bunTopcollider4.GetSubject();
	bunTopColliderSubje4.AddObserver(bunTopCmpt4);

	auto& lettuceObj4 = sceneLevel1.CreateObject("lettuceObj4");
	auto& lettuceCmpt4 = lettuceObj4.AddComponent<IngredientComponent>();
	auto& lettuceSubje4 = lettuceCmpt4.GetSubject();
	lettuceObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider4 = lettuceObj4.AddComponent<RectColliderComponent>();
	lettucecollider4.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	lettuceObj4.SetLocalPosition(477.f, 280.f);
	lettuceCmpt4.SetSpawnLocation(477.f, 280.f);
	auto& lettuceColliderSubje4 = lettucecollider4.GetSubject();
	lettuceColliderSubje4.AddObserver(lettuceCmpt4);

	auto& burgerMeatObj4 = sceneLevel1.CreateObject("burgerMeatObj4");
	auto& burgerMeatCmpt4 = burgerMeatObj4.AddComponent<IngredientComponent>();
	auto& burgerMeatSubje4 = burgerMeatCmpt4.GetSubject();
	burgerMeatObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider4 = burgerMeatObj4.AddComponent<RectColliderComponent>();
	burgerMeatcollider4.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	burgerMeatObj4.SetLocalPosition(477.f, 370.f);
	burgerMeatCmpt4.SetSpawnLocation(477.f, 370.f);
	auto& burgerMeatColliderSubje4 = burgerMeatcollider4.GetSubject();
	burgerMeatColliderSubje4.AddObserver(burgerMeatCmpt4);

	auto& bunBottomObj4 = sceneLevel1.CreateObject("bunBottomObj4");
	auto& bunBottomCmpt4 = bunBottomObj4.AddComponent<IngredientComponent>();
	auto& bunBottomSubje4 = bunBottomCmpt4.GetSubject();
	bunBottomObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider4 = bunBottomObj4.AddComponent<RectColliderComponent>();
	bunBottomcollider4.Init({ 2, 0, 84, 20 }, 1, true, { 133, 0, 133, 255 });
	bunBottomObj4.SetLocalPosition(477.f, 455.f);
	bunBottomCmpt4.SetSpawnLocation(477.f, 455.f);
	auto& bunBottomColliderSubje4 = bunBottomcollider4.GetSubject();
	bunBottomColliderSubje4.AddObserver(bunBottomCmpt4);
#pragma endregion
#pragma endregion

#pragma region PlateSetup
	auto& plateObj1 = sceneLevel1.CreateObject("plateObj1");
	auto& plateCmpt1 = plateObj1.AddComponent<PlateComponent>();
	plateCmpt1.SetIngredientFullCount(4);
	auto& plateSubje1 = plateCmpt1.GetSubject();
	plateObj1.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider1 = plateObj1.AddComponent<RectColliderComponent>();
	plateCollider1.Init({ 0, 8, 104, 4 }, 1, true, { 0, 133, 133, 255 });
	plateObj1.SetLocalPosition(68.f, 800.f);
	auto& plateColliderSubje1 = plateCollider1.GetSubject();
	plateColliderSubje1.AddObserver(plateCmpt1);

	auto& plateObj2 = sceneLevel1.CreateObject("plateObj2");
	auto& plateCmpt2 = plateObj2.AddComponent<PlateComponent>();
	plateCmpt2.SetIngredientFullCount(4);
	auto& plateSubje2 = plateCmpt2.GetSubject();
	plateObj2.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider2 = plateObj2.AddComponent<RectColliderComponent>();
	plateCollider2.Init({ 0, 8, 104, 4 }, 1, true, { 0, 133, 133, 255 });
	plateObj2.SetLocalPosition(202.f, 800.f);
	auto& plateColliderSubje2 = plateCollider2.GetSubject();
	plateColliderSubje2.AddObserver(plateCmpt2);

	auto& plateObj3 = sceneLevel1.CreateObject("plateObj3");
	auto& plateCmpt3 = plateObj3.AddComponent<PlateComponent>();
	plateCmpt3.SetIngredientFullCount(4);
	auto& plateSubje3 = plateCmpt3.GetSubject();
	plateObj3.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider3 = plateObj3.AddComponent<RectColliderComponent>();
	plateCollider3.Init({ 0, 8, 104, 4 }, 1, true, { 0, 133, 133, 255 });
	plateObj3.SetLocalPosition(336.f, 800.f);
	auto& plateColliderSubje3 = plateCollider3.GetSubject();
	plateColliderSubje3.AddObserver(plateCmpt3);

	auto& plateObj4 = sceneLevel1.CreateObject("plateObj4");
	auto& plateCmpt4 = plateObj4.AddComponent<PlateComponent>();
	plateCmpt4.SetIngredientFullCount(4);
	auto& plateSubje4 = plateCmpt4.GetSubject();
	plateObj4.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider4 = plateObj4.AddComponent<RectColliderComponent>();
	plateCollider4.Init({ 0, 8, 104, 4 }, 1, true, { 0, 133, 133, 255 });
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
	livessubje.AddObserver(levelManagercmpt);

	petterPeppersubje.AddObserver(livescmpt);
	sallySaltSubje.AddObserver(livescmpt);

	auto& scoreObject = sceneLevel1.CreateObject("scoreObject");
	auto& scoretextcomp = scoreObject.AddComponent<TextComponent>();
	scoretextcomp.Setup(uiFont);
	auto& scoreCmpt = scoreObject.AddComponent<ScoreComponent>();
	scoreCmpt.SetTextComponent(scoretextcomp);
	scoreObject.SetLocalPosition(uiFontSize * 7.f, uiFontSize * 1.5f);
	auto& scoresubje = scoreCmpt.GetSubject();
	scoresubje.AddObserver(livescmpt);
	levelManagercmpt.SetScoreComponent(&scoreCmpt);

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
	auto& pepperCountTextComp = pepperCountObject.AddComponent<TextComponent>();
	pepperCountTextComp.Setup(uiFont);
	auto& pepperCountCmpt = pepperCountObject.AddComponent<PepperCountComponent>();
	pepperCountCmpt.SetTextComponent(pepperCountTextComp);
	pepperCountObject.SetLocalPosition(windowW - (uiFontSize * 2.f), uiFontSize * 1.5f);
#pragma endregion

#pragma region InputCommandsPlayer
	//peter pepper input
	inputmanager.AddCommand<WalkLeftCommand>(PCController::ControllerButton::Button_DPAD_LEFT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkLeftCommand>(InputManager::KeyboardKey::Key_A, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkRightCommand>(PCController::ControllerButton::Button_DPAD_RIGHT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkRightCommand>(InputManager::KeyboardKey::Key_D, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkUpCommand>(PCController::ControllerButton::Button_DPAD_UP, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkUpCommand>(InputManager::KeyboardKey::Key_W, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkDownCommand>(PCController::ControllerButton::Button_DPAD_DOWN, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkDownCommand>(InputManager::KeyboardKey::Key_S, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);

	auto& throwPepperCmdController1 = inputmanager.AddCommand<ThrowPepperCommand>(PCController::ControllerButton::Button_Square, InputManager::ButtonPressState::OnPressed);
	throwPepperCmdController1.SetPlayer(&peterPepperObj);
	throwPepperCmdController1.SetPepper(&pepperObj);
	throwPepperCmdController1.SetPepperCountComponent(&pepperCountCmpt);
	auto& throwPepperCmdKeyBoard1 = inputmanager.AddCommand<ThrowPepperCommand>(InputManager::KeyboardKey::Key_CTRL_L, InputManager::ButtonPressState::OnPressed);
	throwPepperCmdKeyBoard1.SetPlayer(&peterPepperObj);
	throwPepperCmdKeyBoard1.SetPepper(&pepperObj);
	throwPepperCmdKeyBoard1.SetPepperCountComponent(&pepperCountCmpt);


	//sally salt input
	inputmanager.AddCommand<WalkLeftCommand>(PCController::ControllerButton::Button_DPAD_LEFT, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkLeftCommand>(InputManager::KeyboardKey::Key_ARROW_LEFT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkRightCommand>(PCController::ControllerButton::Button_DPAD_RIGHT, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkRightCommand>(InputManager::KeyboardKey::Key_ARROW_RIGHT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkUpCommand>(PCController::ControllerButton::Button_DPAD_UP, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkUpCommand>(InputManager::KeyboardKey::Key_ARROW_UP, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkDownCommand>(PCController::ControllerButton::Button_DPAD_DOWN, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkDownCommand>(InputManager::KeyboardKey::Key_ARROW_DOWN, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&sallySaltObj);

	auto& throwPepperCmdController2 = inputmanager.AddCommand<ThrowPepperCommand>(PCController::ControllerButton::Button_Square, InputManager::ButtonPressState::OnPressed, 1);
	throwPepperCmdController2.SetPlayer(&sallySaltObj);
	throwPepperCmdController2.SetPepper(&pepperObj);
	throwPepperCmdController2.SetPepperCountComponent(&pepperCountCmpt);

	auto& throwPepperCmdKeyBoard2 = inputmanager.AddCommand<ThrowPepperCommand>(InputManager::KeyboardKey::Key_CTRL_R, InputManager::ButtonPressState::OnPressed);
	throwPepperCmdKeyBoard2.SetPlayer(&sallySaltObj);
	throwPepperCmdKeyBoard2.SetPepper(&pepperObj);
	throwPepperCmdKeyBoard2.SetPepperCountComponent(&pepperCountCmpt);

	//player Hotdog input
	inputmanager.AddCommand<WalkLeftCommand>(PCController::ControllerButton::Button_DPAD_LEFT, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkLeftCommand>(InputManager::KeyboardKey::Key_ARROW_LEFT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkRightCommand>(PCController::ControllerButton::Button_DPAD_RIGHT, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkRightCommand>(InputManager::KeyboardKey::Key_ARROW_RIGHT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkUpCommand>(PCController::ControllerButton::Button_DPAD_UP, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkUpCommand>(InputManager::KeyboardKey::Key_ARROW_UP, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkDownCommand>(PCController::ControllerButton::Button_DPAD_DOWN, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkDownCommand>(InputManager::KeyboardKey::Key_ARROW_DOWN, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&playerHotDogObj);
#pragma endregion

	auto& levelManagersubje = levelManagercmpt.GetSubject();
	levelManagersubje.AddObserver(petercmpt);
	levelManagersubje.AddObserver(sallycmpt);

	levelManagersubje.AddObserver(mrHotDogcmpt1);
	//levelManagersubje.AddObserver(mrHotDogcmpt2);
	//levelManagersubje.AddObserver(mrHotDogcmpt3);
	//levelManagersubje.AddObserver(mrEggcmpt1);

	plateSubje1.AddObserver(levelManagercmpt);
	plateSubje2.AddObserver(levelManagercmpt);
	plateSubje3.AddObserver(levelManagercmpt);
	plateSubje4.AddObserver(levelManagercmpt);

#pragma endregion

	sceneLevel1.LateInit();//intialize the new scene
}

void dae::MainMenuComponent::LoadLevel2()
{
	auto& sceneLevel2 = SceneManager::GetInstance().CreateScene("Level2");
	auto& inputmanager = InputManager::GetInstance();

	auto fpsFont = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 19);

	int windowW{}, windowH{};
	SDL_GetWindowSize(m_pEngine->GetWindow(), &windowW, &windowH);

	auto& backgroundObject = sceneLevel2.CreateObject("backgroundObject");
	backgroundObject.AddComponent<Texture2DComponent>().SetTexture("Level/Level2.png");

	auto& fpsCounterObj = sceneLevel2.CreateObject("fpsCounterObj");
	auto& fpstextComponent = fpsCounterObj.AddComponent<TextComponent>();
	auto& fpsComponent = fpsCounterObj.AddComponent<FPSComponent>();
	fpsComponent.SetTextComponent(fpstextComponent);
	fpsCounterObj.SetLocalPosition(5.f, 5.f);
	fpstextComponent.SetFont(fpsFont);
	fpstextComponent.SetTextColor({ 255, 255, 0 });

	auto& levelManagerObj = sceneLevel2.CreateObject("levelManagerObj");
	auto& levelManagercmpt = levelManagerObj.AddComponent<LevelManager>();
	levelManagercmpt.SetMaxFullPlateCount(4);
	levelManagercmpt.SetLastSceneIndex(3);//there are only 3 levels in the game

	//register the level manager so that it gets updated when the gamemodechanges
	m_pLevelManagers.emplace_back(&levelManagercmpt);

#pragma region Pepper_PeterAndSally
	//pepper object to throw on enemies
	auto& pepperObj = sceneLevel2.CreateObject("pepperObj");
	auto& pepperCmpt = pepperObj.AddComponent<PepperComponent>();
	auto& pepperSpriteManager = pepperObj.AddComponent<SpriteManagerComponent>();
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Left.png", 4, 1, 4, 45, 45);
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Right.png", 4, 1, 4, 45, 45);
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Up.png", 4, 1, 4, 45, 45);
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Down.png", 4, 1, 4, 45, 45);
	auto& pepperCollider = pepperObj.AddComponent<RectColliderComponent>();
	pepperCollider.Init({ 0,0,45,45 }, 2, true);
	pepperObj.SetLocalPosition(-100.f, -100.f);
	pepperCmpt.SetResetPos(-100.f, -100.f);
	pepperCmpt.SetSpriteDuration(1.f);

	auto& peppercollidersubje = pepperCollider.GetSubject();
	peppercollidersubje.AddObserver(pepperCmpt);
	auto& peterPepperObj = sceneLevel2.CreateObject("peterPepperObj");
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
	peterCollider.Init({ 0,1,45,44 }, 2, true);
	peterPepperObj.AddComponent<MovementComponent>();
	petercmpt.SetSpawnLocation(298.f, 609.f);
	peterPepperObj.SetLocalPosition(298.f, 609.f);
	auto& pettercollidersubje = peterCollider.GetSubject();
	pettercollidersubje.AddObserver(petercmpt);
	auto& petterPeppersubje = petercmpt.GetSubject();

	//sally salt
	auto& sallySaltObj = sceneLevel2.CreateObject("sallySaltObj");
	auto& sallycmpt = sallySaltObj.AddComponent<PeterPepper>();
	auto& sallySpriteManager = sallySaltObj.AddComponent<SpriteManagerComponent>();
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Idle.png", 1, 1, 0, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Walking_Left.png", 4, 1, 16, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Walking_Right.png", 4, 1, 16, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Walking_Up.png", 4, 1, 16, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Walking_Down.png", 4, 1, 16, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Death.png", 6, 1, 2, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Won.png", 2, 1, 2, 45, 45);

	auto& sallyCollider = sallySaltObj.AddComponent<RectColliderComponent>();
	sallyCollider.Init({ 0,1,45,44 }, 2, true);
	sallySaltObj.AddComponent<MovementComponent>();
	sallycmpt.SetSpawnLocation(298.f, 162.f);
	sallySaltObj.SetLocalPosition(298.f, 162.f);

	auto& sallyColliderSubje = sallyCollider.GetSubject();
	sallyColliderSubje.AddObserver(sallycmpt);
	auto& sallySaltSubje = sallycmpt.GetSubject();

	//the 2nd level manager will enable or disable the player(s) depending on the mode that was selected,
	//so by default its disabled
	peterPepperObj.SetEnabledState(false);
	sallySaltObj.SetEnabledState(false);
	pepperObj.SetEnabledState(false);

	levelManagercmpt.SetNormalPlayer(&peterPepperObj);
	levelManagercmpt.SetCoopPlayer(&sallySaltObj);
	levelManagercmpt.SetPepperObject(&pepperObj);
#pragma endregion

#pragma region Enemies
#pragma region EnemyPlayer
	auto& playerHotDogObj = sceneLevel2.CreateObject("playerHotDogObj");
	auto& playerHotDogCmpt = playerHotDogObj.AddComponent<MrHotDog>();
	playerHotDogCmpt.SetControlledByHuman(true);
	auto& playerHotDogSpriteManager = playerHotDogObj.AddComponent<SpriteManagerComponent>();
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Idle.png", 1, 1, 0, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Left.png", 2, 1, 8, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Right.png", 2, 1, 8, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Up.png", 2, 1, 8, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Down.png", 2, 1, 8, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Death.png", 4, 1, 10, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Peppered.png", 2, 1, 8, 45, 45);
	playerHotDogCmpt.SetDeathDuration(4 / 10.f);//4frames / 10fps

	auto& playerHotDogCollider = playerHotDogObj.AddComponent<RectColliderComponent>();
	playerHotDogCollider.Init({ 0,1,45,44 }, 2, true);
	playerHotDogObj.AddComponent<MovementComponent>();
	playerHotDogCmpt.SetRespawnPosAndWalkDirection(30, 162, EnemyControllerComponent::MovementState::Right);
	auto& playerHotDogcollidersubje = playerHotDogCollider.GetSubject();
	playerHotDogcollidersubje.AddObserver(playerHotDogCmpt);
#pragma endregion
#pragma region Enemy1
	auto& mrHotDogObj1 = sceneLevel2.CreateObject("mrHotDogObj1");
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
	mrHotDogCollider1.Init({ 0,1,45,44 }, 2, true);
	mrHotDogObj1.AddComponent<MovementComponent>();
	auto& mrHotDogController1 = mrHotDogObj1.AddComponent<EnemyControllerComponent>();
	mrHotDogcmpt1.SetRespawnPosAndWalkDirection(30, 162, EnemyControllerComponent::MovementState::Right);
	auto& mrHotDogcollidersubje = mrHotDogCollider1.GetSubject();
	mrHotDogcollidersubje.AddObserver(mrHotDogcmpt1);
#pragma endregion



	//always set the first player, the 2nd player will be set or unset in the level manager once the leve becomes active
	mrHotDogController1.SetPlayer1(&peterPepperObj);

	//register enemies to the levelmanager
	levelManagercmpt.SetEnemyPlayer(&playerHotDogObj);

	levelManagercmpt.AddEnemy(&mrHotDogObj1);
	//levelManagercmpt.AddEnemy(&mrHotDogObj2);
	//levelManagercmpt.AddEnemy(&mrHotDogObj3);
	//levelManagercmpt.AddEnemy(&mrPickleObj1);


	//the 2nd level manager will enable or disable the enemies depending on the mode that was selected,
	//so by default its disabled
	playerHotDogObj.SetEnabledState(false);

	mrHotDogObj1.SetEnabledState(false);
	//mrHotDogObj2.SetEnabledState(false);
	//mrHotDogObj3.SetEnabledState(false);
	//mrEggObj1.SetEnabledState(false);

	//let the enemies observe the player(s)
	petterPeppersubje.AddObserver(playerHotDogCmpt);

	petterPeppersubje.AddObserver(mrHotDogcmpt1);
	//petterPeppersubje.AddObserver(mrHotDogcmpt2);
	//petterPeppersubje.AddObserver(mrHotDogcmpt3);
	//petterPeppersubje.AddObserver(mrEggcmpt1);

	sallySaltSubje.AddObserver(playerHotDogCmpt);

	sallySaltSubje.AddObserver(mrHotDogcmpt1);
	//sallySaltSubje.AddObserver(mrHotDogcmpt2);
	//sallySaltSubje.AddObserver(mrHotDogcmpt3);
	//sallySaltSubje.AddObserver(mrEggcmpt1);
#pragma endregion

#pragma region LadderSetup
	auto& ladderObj1 = sceneLevel2.CreateObject("ladderObj1");
	ladderObj1.SetLocalPosition(51.f, 189.f);
	ladderObj1.AddComponent<LadderComponent>();
	auto& ladder1colliderCmpt = ladderObj1.AddComponent<RectColliderComponent>();
	ladder1colliderCmpt.Init({ 0, 0, 3, 197 }, 2, true, { 255, 255, 0, 128 });

	auto& ladderObj2 = sceneLevel2.CreateObject("ladderObj2");
	ladderObj2.SetLocalPosition(117.f, 189.f);
	ladderObj2.AddComponent<LadderComponent>();
	auto& ladder2colliderCmpt = ladderObj2.AddComponent<RectColliderComponent>();
	ladder2colliderCmpt.Init({ 0, 0, 3, 197 }, 2, true, { 255, 255, 0, 128 });

	auto& ladderObj3 = sceneLevel2.CreateObject("ladderObj3");
	ladderObj3.SetLocalPosition(184.f, 189.f);
	ladderObj3.AddComponent<LadderComponent>();
	auto& ladder3colliderCmpt = ladderObj3.AddComponent<RectColliderComponent>();
	ladder3colliderCmpt.Init({ 0, 0, 4, 197 }, 2, true, { 255, 255, 0, 128 });

	auto& ladderObj4 = sceneLevel2.CreateObject("ladderObj4");
	ladderObj4.SetLocalPosition(251.f, 189.f);
	ladderObj4.AddComponent<LadderComponent>();
	auto& ladder4colliderCmpt = ladderObj4.AddComponent<RectColliderComponent>();
	ladder4colliderCmpt.Init({ 0, 0, 4, 376 }, 2, true, { 255, 255, 0, 128 });

	auto& ladderObj5 = sceneLevel2.CreateObject("ladderObj5");
	ladderObj5.SetLocalPosition(318.f, 189.f);
	ladderObj5.AddComponent<LadderComponent>();
	auto& ladder5colliderCmpt = ladderObj5.AddComponent<RectColliderComponent>();
	ladder5colliderCmpt.Init({ 0, 0, 4, 465 }, 2, true, { 255, 255, 0, 128 });

	auto& ladderObj6 = sceneLevel2.CreateObject("ladderObj6");
	ladderObj6.SetLocalPosition(385.f, 189.f);
	ladderObj6.AddComponent<LadderComponent>();
	auto& ladder6colliderCmpt = ladderObj6.AddComponent<RectColliderComponent>();
	ladder6colliderCmpt.Init({ 0, 0, 4, 376 }, 2, true, { 255, 255, 0, 128 });

	auto& ladderObj7 = sceneLevel2.CreateObject("ladderObj7");
	ladderObj7.SetLocalPosition(452.f, 189.f);
	ladderObj7.AddComponent<LadderComponent>();
	auto& ladder7colliderCmpt = ladderObj7.AddComponent<RectColliderComponent>();
	ladder7colliderCmpt.Init({ 0, 0, 4, 197 }, 2, true, { 255, 255, 0, 128 });

	auto& ladderObj8 = sceneLevel2.CreateObject("ladderObj8");
	ladderObj8.SetLocalPosition(519.f, 189.f);
	ladderObj8.AddComponent<LadderComponent>();
	auto& ladder8colliderCmpt = ladderObj8.AddComponent<RectColliderComponent>();
	ladder8colliderCmpt.Init({ 0, 0, 4, 197 }, 2, true, { 255, 255, 0, 128 });

	auto& ladderObj9 = sceneLevel2.CreateObject("ladderObj9");
	ladderObj9.SetLocalPosition(586.f, 189.f);
	ladderObj9.AddComponent<LadderComponent>();
	auto& ladder9colliderCmpt = ladderObj9.AddComponent<RectColliderComponent>();
	ladder9colliderCmpt.Init({ 0, 0, 3, 197 }, 2, true, { 255, 255, 0, 128 });
#pragma endregion

#pragma region WalkingPlatformSetup
	auto& walkPlatformObj1 = sceneLevel2.CreateObject("walkPlatformObj1");
	walkPlatformObj1.SetLocalPosition(30.f, 202.f);
	walkPlatformObj1.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform1colliderCmpt = walkPlatformObj1.AddComponent<RectColliderComponent>();
	walkPlatform1colliderCmpt.Init({ 0, 0, 580, 4 }, 2, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj2 = sceneLevel2.CreateObject("walkPlatformObj2");
	walkPlatformObj2.SetLocalPosition(30.f, 247);
	walkPlatformObj2.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform2colliderCmpt = walkPlatformObj2.AddComponent<RectColliderComponent>();
	walkPlatform2colliderCmpt.Init({ 0, 0, 310, 4 }, 2, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj3 = sceneLevel2.CreateObject("walkPlatformObj3");
	walkPlatformObj3.SetLocalPosition(30.f, 292.f);
	walkPlatformObj3.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform3colliderCmpt = walkPlatformObj3.AddComponent<RectColliderComponent>();
	walkPlatform3colliderCmpt.Init({ 0, 0, 175, 4 }, 2, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj4 = sceneLevel2.CreateObject("walkPlatformObj4");
	walkPlatformObj4.SetLocalPosition(298.f, 292.f);
	walkPlatformObj4.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform4colliderCmpt = walkPlatformObj4.AddComponent<RectColliderComponent>();
	walkPlatform4colliderCmpt.Init({ 0, 0, 310, 4 }, 2, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj5 = sceneLevel2.CreateObject("walkPlatformObj5");
	walkPlatformObj5.SetLocalPosition(165.f, 337.f);
	walkPlatformObj5.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform5colliderCmpt = walkPlatformObj5.AddComponent<RectColliderComponent>();
	walkPlatform5colliderCmpt.Init({ 0, 0, 176, 4 }, 2, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj6 = sceneLevel2.CreateObject("walkPlatformObj6");
	walkPlatformObj6.SetLocalPosition(432.f, 337.f);
	walkPlatformObj6.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform6colliderCmpt = walkPlatformObj6.AddComponent<RectColliderComponent>();
	walkPlatform6colliderCmpt.Init({ 0, 0, 176, 4 }, 2, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj7 = sceneLevel2.CreateObject("walkPlatformObj7");
	walkPlatformObj7.SetLocalPosition(30.f, 382.f);
	walkPlatformObj7.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform7colliderCmpt = walkPlatformObj7.AddComponent<RectColliderComponent>();
	walkPlatform7colliderCmpt.Init({ 0, 0, 580, 4 }, 2, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj8 = sceneLevel2.CreateObject("walkPlatformObj8");
	walkPlatformObj8.SetLocalPosition(298.f, 425.f);
	walkPlatformObj8.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform8colliderCmpt = walkPlatformObj8.AddComponent<RectColliderComponent>();
	walkPlatform8colliderCmpt.Init({ 0, 0, 175, 4 }, 2, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj9 = sceneLevel2.CreateObject("walkPlatformObj9");
	walkPlatformObj9.SetLocalPosition(165.f, 470.f);
	walkPlatformObj9.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform9colliderCmpt = walkPlatformObj9.AddComponent<RectColliderComponent>();
	walkPlatform9colliderCmpt.Init({ 0, 0, 175, 4 }, 2, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj10 = sceneLevel2.CreateObject("walkPlatformObj10");
	walkPlatformObj10.SetLocalPosition(298.f, 515);
	walkPlatformObj10.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform10colliderCmpt = walkPlatformObj10.AddComponent<RectColliderComponent>();
	walkPlatform10colliderCmpt.Init({ 0, 0, 175, 4 }, 2, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj11 = sceneLevel2.CreateObject("walkPlatformObj11");
	walkPlatformObj11.SetLocalPosition(165.f, 560.f);
	walkPlatformObj11.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform11colliderCmpt = walkPlatformObj11.AddComponent<RectColliderComponent>();
	walkPlatform11colliderCmpt.Init({ 0, 0, 310, 4 }, 2, true, { 0, 255, 0, 128 });
#pragma endregion

#pragma region IngredientsSetup
#pragma region burger1
	auto& bunTopObj1 = sceneLevel2.CreateObject("bunTopObj1");
	auto& bunTopCmpt1 = bunTopObj1.AddComponent<IngredientComponent>();
	auto& bunTopSubje1 = bunTopCmpt1.GetSubject();
	bunTopObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider1 = bunTopObj1.AddComponent<RectColliderComponent>();
	bunTopcollider1.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	bunTopObj1.SetLocalPosition(75.f, 190.f);
	bunTopCmpt1.SetSpawnLocation(75.f, 190.f);
	auto& bunTopColliderSubje1 = bunTopcollider1.GetSubject();
	bunTopColliderSubje1.AddObserver(bunTopCmpt1);

	auto& lettuceObj1 = sceneLevel2.CreateObject("lettuceObj1");
	auto& lettuceCmpt1 = lettuceObj1.AddComponent<IngredientComponent>();
	auto& lettuceSubje1 = lettuceCmpt1.GetSubject();
	lettuceObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider1 = lettuceObj1.AddComponent<RectColliderComponent>();
	lettucecollider1.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	lettuceObj1.SetLocalPosition(75.f, 235.f);
	lettuceCmpt1.SetSpawnLocation(75.f, 235.f);
	auto& lettuceColliderSubje1 = lettucecollider1.GetSubject();
	lettuceColliderSubje1.AddObserver(lettuceCmpt1);

	auto& cheeseObj1 = sceneLevel2.CreateObject("cheeseObj1");
	auto& cheeseCmpt1 = cheeseObj1.AddComponent<IngredientComponent>();
	auto& cheeseSubje1 = cheeseCmpt1.GetSubject();
	cheeseObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Cheese.png", 88.f, 22.f);
	auto& cheesecollider1 = cheeseObj1.AddComponent<RectColliderComponent>();
	cheesecollider1.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	cheeseObj1.SetLocalPosition(75.f, 280.f);
	cheeseCmpt1.SetSpawnLocation(75.f, 280.f);
	auto& cheeseColliderSubje1 = cheesecollider1.GetSubject();
	cheeseColliderSubje1.AddObserver(cheeseCmpt1);

	auto& bunBottomObj1 = sceneLevel2.CreateObject("bunBottomObj1");
	auto& bunBottomCmpt1 = bunBottomObj1.AddComponent<IngredientComponent>();
	auto& bunBottomSubje1 = bunBottomCmpt1.GetSubject();
	bunBottomObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider1 = bunBottomObj1.AddComponent<RectColliderComponent>();
	bunBottomcollider1.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	bunBottomObj1.SetLocalPosition(75.f, 370.f);
	bunBottomCmpt1.SetSpawnLocation(75.f, 370.f);
	auto& bunBottomColliderSubje1 = bunBottomcollider1.GetSubject();
	bunBottomColliderSubje1.AddObserver(bunBottomCmpt1);
#pragma endregion
#pragma region burger2
	auto& bunTopObj2 = sceneLevel2.CreateObject("bunTopObj2");
	auto& bunTopCmpt2 = bunTopObj2.AddComponent<IngredientComponent>();
	auto& bunTopSubje2 = bunTopCmpt2.GetSubject();
	bunTopObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider2 = bunTopObj2.AddComponent<RectColliderComponent>();
	bunTopcollider2.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	bunTopObj2.SetLocalPosition(209.f, 190.f);
	bunTopCmpt2.SetSpawnLocation(209.f, 190.f);
	auto& bunTopColliderSubje2 = bunTopcollider2.GetSubject();
	bunTopColliderSubje2.AddObserver(bunTopCmpt2);

	auto& cheeseObj2 = sceneLevel2.CreateObject("cheeseObj2");
	auto& cheeseCmpt2 = cheeseObj2.AddComponent<IngredientComponent>();
	auto& cheeseSubje2 = cheeseCmpt2.GetSubject();
	cheeseObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Cheese.png", 88.f, 22.f);
	auto& cheesecollider2 = cheeseObj2.AddComponent<RectColliderComponent>();
	cheesecollider2.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	cheeseObj2.SetLocalPosition(209.f, 235.f);
	cheeseCmpt2.SetSpawnLocation(209.f, 235.f);
	auto& cheeseColliderSubje2 = cheesecollider2.GetSubject();
	cheeseColliderSubje2.AddObserver(cheeseCmpt2);

	auto& lettuceObj2 = sceneLevel2.CreateObject("lettuceObj2");
	auto& lettuceCmpt2 = lettuceObj2.AddComponent<IngredientComponent>();
	auto& lettuceSubje2 = lettuceCmpt2.GetSubject();
	lettuceObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider2 = lettuceObj2.AddComponent<RectColliderComponent>();
	lettucecollider2.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	lettuceObj2.SetLocalPosition(209.f, 325.f);
	lettuceCmpt2.SetSpawnLocation(209.f, 325.f);
	auto& lettuceColliderSubje2 = lettucecollider2.GetSubject();
	lettuceColliderSubje2.AddObserver(lettuceCmpt2);

	auto& bunBottomObj2 = sceneLevel2.CreateObject("bunBottomObj2");
	auto& bunBottomCmpt2 = bunBottomObj2.AddComponent<IngredientComponent>();
	auto& bunBottomSubje2 = bunBottomCmpt2.GetSubject();
	bunBottomObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider2 = bunBottomObj2.AddComponent<RectColliderComponent>();
	bunBottomcollider2.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	bunBottomObj2.SetLocalPosition(209.f, 550.f);
	bunBottomCmpt2.SetSpawnLocation(209.f, 550.f);
	auto& bunBottomColliderSubje2 = bunBottomcollider2.GetSubject();
	bunBottomColliderSubje2.AddObserver(bunBottomCmpt2);
#pragma endregion
#pragma region burger3
	auto& bunTopObj3 = sceneLevel2.CreateObject("bunTopObj3");
	auto& bunTopCmpt3 = bunTopObj3.AddComponent<IngredientComponent>();
	auto& bunTopSubje3 = bunTopCmpt3.GetSubject();
	bunTopObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider3 = bunTopObj3.AddComponent<RectColliderComponent>();
	bunTopcollider3.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	bunTopObj3.SetLocalPosition(343.f, 190.f);
	bunTopCmpt3.SetSpawnLocation(343.f, 190.f);
	auto& bunTopColliderSubje3 = bunTopcollider3.GetSubject();
	bunTopColliderSubje3.AddObserver(bunTopCmpt3);

	auto& cheeseObj3 = sceneLevel2.CreateObject("cheeseObj3");
	auto& cheeseCmpt3 = cheeseObj3.AddComponent<IngredientComponent>();
	auto& cheeseSubje3 = cheeseCmpt3.GetSubject();
	cheeseObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Cheese.png", 88.f, 22.f);
	auto& cheesecollider3 = cheeseObj3.AddComponent<RectColliderComponent>();
	cheesecollider3.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	cheeseObj3.SetLocalPosition(343.f, 410.f);
	cheeseCmpt3.SetSpawnLocation(343.f, 410.f);
	auto& cheeseColliderSubje3 = cheesecollider3.GetSubject();
	cheeseColliderSubje3.AddObserver(cheeseCmpt3);

	auto& lettuceObj3 = sceneLevel2.CreateObject("lettuceObj3");
	auto& lettuceCmpt3 = lettuceObj3.AddComponent<IngredientComponent>();
	auto& lettuceSubje3 = lettuceCmpt3.GetSubject();
	lettuceObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider3 = lettuceObj3.AddComponent<RectColliderComponent>();
	lettucecollider3.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	lettuceObj3.SetLocalPosition(343.f, 505.f);
	lettuceCmpt3.SetSpawnLocation(343.f, 505.f);
	auto& lettuceColliderSubje3 = lettucecollider3.GetSubject();
	lettuceColliderSubje3.AddObserver(lettuceCmpt3);

	auto& bunBottomObj3 = sceneLevel2.CreateObject("bunBottomObj3");
	auto& bunBottomCmpt3 = bunBottomObj3.AddComponent<IngredientComponent>();
	auto& bunBottomSubje3 = bunBottomCmpt3.GetSubject();
	bunBottomObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider3 = bunBottomObj3.AddComponent<RectColliderComponent>();
	bunBottomcollider3.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	bunBottomObj3.SetLocalPosition(343.f, 550.f);
	bunBottomCmpt3.SetSpawnLocation(343.f, 550.f);
	auto& bunBottomColliderSubje3 = bunBottomcollider3.GetSubject();
	bunBottomColliderSubje3.AddObserver(bunBottomCmpt3);
#pragma endregion
#pragma region burger4
	auto& bunTopObj4 = sceneLevel2.CreateObject("bunTopObj4");
	auto& bunTopCmpt4 = bunTopObj4.AddComponent<IngredientComponent>();
	auto& bunTopSubje4 = bunTopCmpt4.GetSubject();
	bunTopObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider4 = bunTopObj4.AddComponent<RectColliderComponent>();
	bunTopcollider4.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	bunTopObj4.SetLocalPosition(477.f, 190.f);
	bunTopCmpt4.SetSpawnLocation(477.f, 190.f);
	auto& bunTopColliderSubje4 = bunTopcollider4.GetSubject();
	bunTopColliderSubje4.AddObserver(bunTopCmpt4);

	auto& lettuceObj4 = sceneLevel2.CreateObject("lettuceObj4");
	auto& lettuceCmpt4 = lettuceObj4.AddComponent<IngredientComponent>();
	auto& lettuceSubje4 = lettuceCmpt4.GetSubject();
	lettuceObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider4 = lettuceObj4.AddComponent<RectColliderComponent>();
	lettucecollider4.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	lettuceObj4.SetLocalPosition(477.f, 280.f);
	lettuceCmpt4.SetSpawnLocation(477.f, 280.f);
	auto& lettuceColliderSubje4 = lettucecollider4.GetSubject();
	lettuceColliderSubje4.AddObserver(lettuceCmpt4);

	auto& cheeseObj4 = sceneLevel2.CreateObject("cheeseObj4");
	auto& cheeseCmpt4 = cheeseObj4.AddComponent<IngredientComponent>();
	auto& cheeseSubje4 = cheeseCmpt4.GetSubject();
	cheeseObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Cheese.png", 88.f, 22.f);
	auto& cheesecollider4 = cheeseObj4.AddComponent<RectColliderComponent>();
	cheesecollider4.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	cheeseObj4.SetLocalPosition(477.f, 325.f);
	cheeseCmpt4.SetSpawnLocation(477.f, 325.f);
	auto& cheeseColliderSubje4 = cheesecollider4.GetSubject();
	cheeseColliderSubje4.AddObserver(cheeseCmpt4);

	auto& bunBottomObj4 = sceneLevel2.CreateObject("bunBottomObj4");
	auto& bunBottomCmpt4 = bunBottomObj4.AddComponent<IngredientComponent>();
	auto& bunBottomSubje4 = bunBottomCmpt4.GetSubject();
	bunBottomObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider4 = bunBottomObj4.AddComponent<RectColliderComponent>();
	bunBottomcollider4.Init({ 2, 0, 84, 20 }, 2, true, { 133, 0, 133, 255 });
	bunBottomObj4.SetLocalPosition(477.f, 370.f);
	bunBottomCmpt4.SetSpawnLocation(477.f, 370.f);
	auto& bunBottomColliderSubje4 = bunBottomcollider4.GetSubject();
	bunBottomColliderSubje4.AddObserver(bunBottomCmpt4);
#pragma endregion
#pragma endregion

#pragma region PlateSetup
	auto& plateObj1 = sceneLevel2.CreateObject("plateObj1");
	auto& plateCmpt1 = plateObj1.AddComponent<PlateComponent>();
	plateCmpt1.SetIngredientFullCount(4);
	auto& plateSubje1 = plateCmpt1.GetSubject();
	plateObj1.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider1 = plateObj1.AddComponent<RectColliderComponent>();
	plateCollider1.Init({ 0, 8, 104, 4 }, 2, true, { 0, 133, 133, 255 });
	plateObj1.SetLocalPosition(68.f, 600.f);
	auto& plateColliderSubje1 = plateCollider1.GetSubject();
	plateColliderSubje1.AddObserver(plateCmpt1);

	auto& plateObj2 = sceneLevel2.CreateObject("plateObj2");
	auto& plateCmpt2 = plateObj2.AddComponent<PlateComponent>();
	plateCmpt2.SetIngredientFullCount(4);
	auto& plateSubje2 = plateCmpt2.GetSubject();
	plateObj2.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider2 = plateObj2.AddComponent<RectColliderComponent>();
	plateCollider2.Init({ 0, 8, 104, 4 }, 2, true, { 0, 133, 133, 255 });
	plateObj2.SetLocalPosition(202.f, 800.f);
	auto& plateColliderSubje2 = plateCollider2.GetSubject();
	plateColliderSubje2.AddObserver(plateCmpt2);

	auto& plateObj3 = sceneLevel2.CreateObject("plateObj3");
	auto& plateCmpt3 = plateObj3.AddComponent<PlateComponent>();
	plateCmpt3.SetIngredientFullCount(4);
	auto& plateSubje3 = plateCmpt3.GetSubject();
	plateObj3.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider3 = plateObj3.AddComponent<RectColliderComponent>();
	plateCollider3.Init({ 0, 8, 104, 4 }, 2, true, { 0, 133, 133, 255 });
	plateObj3.SetLocalPosition(336.f, 800.f);
	auto& plateColliderSubje3 = plateCollider3.GetSubject();
	plateColliderSubje3.AddObserver(plateCmpt3);

	auto& plateObj4 = sceneLevel2.CreateObject("plateObj4");
	auto& plateCmpt4 = plateObj4.AddComponent<PlateComponent>();
	plateCmpt4.SetIngredientFullCount(4);
	auto& plateSubje4 = plateCmpt4.GetSubject();
	plateObj4.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider4 = plateObj4.AddComponent<RectColliderComponent>();
	plateCollider4.Init({ 0, 8, 104, 4 }, 2, true, { 0, 133, 133, 255 });
	plateObj4.SetLocalPosition(470.f, 600.f);
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
	auto& oneUpTextObject = sceneLevel2.CreateObject("oneUpTextObject");
	oneUpTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 255, 0, 0 }, "1UP");
	oneUpTextObject.SetLocalPosition(float(uiFontSize * 4), 5);

	auto& hiScoreTextObject = sceneLevel2.CreateObject("hiScoreTextObject");
	hiScoreTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 255, 0, 0 }, "HI-SCORE");
	hiScoreTextObject.SetLocalPosition(float(uiFontSize * 4) + (uiFontSize * 5), 5);

	auto& pepperTextObject = sceneLevel2.CreateObject("pepperTextObject");
	pepperTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 0, 255, 0 }, "PEPPER");
	pepperTextObject.SetLocalPosition(float(windowW - (uiFontSize * 7)), 5);
#pragma endregion

	//
	//Dynamic UI
	//
	auto& peterLivesObj = sceneLevel2.CreateObject("peterLivesObj");
	auto& livescmpt = peterLivesObj.AddComponent<LivesComponent>();
	livescmpt.SetTexture("UI/Peter_Life_Icon.png", 3.25f, 3.25f);
	peterLivesObj.SetLocalPosition(10.f, windowH - 40.f);
	auto& livessubje = livescmpt.GetSubject();
	livessubje.AddObserver(levelManagercmpt);
	petterPeppersubje.AddObserver(livescmpt);
	sallySaltSubje.AddObserver(livescmpt);

	auto& scoreObject = sceneLevel2.CreateObject("scoreObject");
	auto& scoretextcomp = scoreObject.AddComponent<TextComponent>();
	scoretextcomp.Setup(uiFont);
	auto& scoreCmpt = scoreObject.AddComponent<ScoreComponent>();
	scoreCmpt.SetTextComponent(scoretextcomp);
	scoreObject.SetLocalPosition(uiFontSize * 7.f, uiFontSize * 1.5f);
	auto& scoresubje = scoreCmpt.GetSubject();
	scoresubje.AddObserver(livescmpt);
	levelManagercmpt.SetScoreComponent(&scoreCmpt);

#pragma region ScoreObservingIngredients
	bunBottomSubje1.AddObserver(scoreCmpt);
	bunBottomSubje2.AddObserver(scoreCmpt);
	bunBottomSubje3.AddObserver(scoreCmpt);
	bunBottomSubje4.AddObserver(scoreCmpt);
	cheeseSubje1.AddObserver(scoreCmpt);
	cheeseSubje2.AddObserver(scoreCmpt);
	cheeseSubje3.AddObserver(scoreCmpt);
	cheeseSubje4.AddObserver(scoreCmpt);
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

	auto& hiScoreObject = sceneLevel2.CreateObject("scoreObject");
	auto& hiScoretextcomp = hiScoreObject.AddComponent<TextComponent>();
	hiScoretextcomp.Setup(uiFont);
	auto& hiScoreCmpt = hiScoreObject.AddComponent<HiScoreComponent>();
	hiScoreCmpt.SetTextComponent(hiScoretextcomp);
	hiScoreCmpt.SetScoreComponent(scoreCmpt);
	hiScoreObject.SetLocalPosition(uiFontSize * 15.f, uiFontSize * 1.5f);

	auto& pepperCountObject = sceneLevel2.CreateObject("pepperCountObject");
	auto& pepperCountTextComp = pepperCountObject.AddComponent<TextComponent>();
	pepperCountTextComp.Setup(uiFont);
	auto& pepperCountCmpt = pepperCountObject.AddComponent<PepperCountComponent>();
	pepperCountCmpt.SetTextComponent(pepperCountTextComp);
	pepperCountObject.SetLocalPosition(windowW - (uiFontSize * 2.f), uiFontSize * 1.5f);
#pragma endregion

#pragma region InputCommandsPlayer
	//peter pepper input
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
	throwPepperCmdController1.SetPepperCountComponent(&pepperCountCmpt);
	auto& throwPepperCmdKeyBoard1 = inputmanager.AddCommand<ThrowPepperCommand>(InputManager::KeyboardKey::Key_CTRL_L, InputManager::ButtonPressState::OnPressed);
	throwPepperCmdKeyBoard1.SetPlayer(&peterPepperObj);
	throwPepperCmdKeyBoard1.SetPepper(&pepperObj);
	throwPepperCmdKeyBoard1.SetPepperCountComponent(&pepperCountCmpt);

	//sally salt input
	inputmanager.AddCommand<WalkLeftCommand>(PCController::ControllerButton::Button_DPAD_LEFT, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkLeftCommand>(InputManager::KeyboardKey::Key_ARROW_LEFT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkRightCommand>(PCController::ControllerButton::Button_DPAD_RIGHT, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkRightCommand>(InputManager::KeyboardKey::Key_ARROW_RIGHT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkUpCommand>(PCController::ControllerButton::Button_DPAD_UP, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkUpCommand>(InputManager::KeyboardKey::Key_ARROW_UP, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkDownCommand>(PCController::ControllerButton::Button_DPAD_DOWN, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkDownCommand>(InputManager::KeyboardKey::Key_ARROW_DOWN, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&sallySaltObj);

	auto& throwPepperCmdController2 = inputmanager.AddCommand<ThrowPepperCommand>(PCController::ControllerButton::Button_Square, InputManager::ButtonPressState::OnPressed, 1);
	throwPepperCmdController2.SetPlayer(&sallySaltObj);
	throwPepperCmdController2.SetPepper(&pepperObj);
	throwPepperCmdController2.SetPepperCountComponent(&pepperCountCmpt);

	auto& throwPepperCmdKeyBoard2 = inputmanager.AddCommand<ThrowPepperCommand>(InputManager::KeyboardKey::Key_CTRL_R, InputManager::ButtonPressState::OnPressed);
	throwPepperCmdKeyBoard2.SetPlayer(&sallySaltObj);
	throwPepperCmdKeyBoard2.SetPepper(&pepperObj);
	throwPepperCmdKeyBoard2.SetPepperCountComponent(&pepperCountCmpt);

	//player Hotdog input
	inputmanager.AddCommand<WalkLeftCommand>(PCController::ControllerButton::Button_DPAD_LEFT, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkLeftCommand>(InputManager::KeyboardKey::Key_ARROW_LEFT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkRightCommand>(PCController::ControllerButton::Button_DPAD_RIGHT, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkRightCommand>(InputManager::KeyboardKey::Key_ARROW_RIGHT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkUpCommand>(PCController::ControllerButton::Button_DPAD_UP, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkUpCommand>(InputManager::KeyboardKey::Key_ARROW_UP, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkDownCommand>(PCController::ControllerButton::Button_DPAD_DOWN, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkDownCommand>(InputManager::KeyboardKey::Key_ARROW_DOWN, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&playerHotDogObj);
#pragma endregion

	auto& levelManagersubje = levelManagercmpt.GetSubject();
	levelManagersubje.AddObserver(petercmpt);
	levelManagersubje.AddObserver(sallycmpt);

	levelManagersubje.AddObserver(mrHotDogcmpt1);
	//levelManagersubje.AddObserver(mrHotDogcmpt2);
	//levelManagersubje.AddObserver(mrHotDogcmpt3);
	//levelManagersubje.AddObserver(mrEggcmpt1);

	plateSubje1.AddObserver(levelManagercmpt);
	plateSubje2.AddObserver(levelManagercmpt);
	plateSubje3.AddObserver(levelManagercmpt);
	plateSubje4.AddObserver(levelManagercmpt);

	sceneLevel2.LateInit();//intialize the new scene
}

void dae::MainMenuComponent::LoadLevel3()
{
	auto& sceneLevel3 = SceneManager::GetInstance().CreateScene("Level3");
	auto& inputmanager = InputManager::GetInstance();

	auto fpsFont = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 19);

	int windowW{}, windowH{};
	SDL_GetWindowSize(m_pEngine->GetWindow(), &windowW, &windowH);

	auto& backgroundObject = sceneLevel3.CreateObject("backgroundObject");
	backgroundObject.AddComponent<Texture2DComponent>().SetTexture("Level/Level3.png");

	auto& fpsCounterObj = sceneLevel3.CreateObject("fpsCounterObj");
	auto& fpstextComponent = fpsCounterObj.AddComponent<TextComponent>();
	auto& fpsComponent = fpsCounterObj.AddComponent<FPSComponent>();
	fpsComponent.SetTextComponent(fpstextComponent);
	fpsCounterObj.SetLocalPosition(5.f, 5.f);
	fpstextComponent.SetFont(fpsFont);
	fpstextComponent.SetTextColor({ 255, 0, 0 });

	auto& levelManagerObj = sceneLevel3.CreateObject("levelManagerObj");
	auto& levelManagercmpt = levelManagerObj.AddComponent<LevelManager>();
	levelManagercmpt.SetMaxFullPlateCount(6);
	levelManagercmpt.SetLastSceneIndex(3);//there are only 3 levels in the game

	//register the level manager so that it gets updated when the gamemodechanges
	m_pLevelManagers.emplace_back(&levelManagercmpt);

#pragma region Pepper_PeterAndSally
	//pepper object to throw on enemies
	auto& pepperObj = sceneLevel3.CreateObject("pepperObj");
	auto& pepperCmpt = pepperObj.AddComponent<PepperComponent>();
	auto& pepperSpriteManager = pepperObj.AddComponent<SpriteManagerComponent>();
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Left.png", 4, 1, 4, 45, 45);
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Right.png", 4, 1, 4, 45, 45);
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Up.png", 4, 1, 4, 45, 45);
	pepperSpriteManager.AddSprite("Misc/Pepper/Pepper_Down.png", 4, 1, 4, 45, 45);
	auto& pepperCollider = pepperObj.AddComponent<RectColliderComponent>();
	pepperCollider.Init({ 0,0,45,45 }, 3, true);
	pepperObj.SetLocalPosition(-100.f, -100.f);
	pepperCmpt.SetResetPos(-100.f, -100.f);
	pepperCmpt.SetSpriteDuration(1.f);

	//PeterPepper
	auto& peppercollidersubje = pepperCollider.GetSubject();
	peppercollidersubje.AddObserver(pepperCmpt);
	auto& peterPepperObj = sceneLevel3.CreateObject("peterPepperObj");
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
	peterCollider.Init({ 0,1,45,44 }, 3, true);
	peterPepperObj.AddComponent<MovementComponent>();
	peterPepperObj.SetLocalPosition(298.f, 609.f);
	petercmpt.SetSpawnLocation(298.f, 609.f);
	auto& pettercollidersubje = peterCollider.GetSubject();
	pettercollidersubje.AddObserver(petercmpt);
	auto& petterPeppersubje = petercmpt.GetSubject();

	//Sally Salt
	auto& sallySaltObj = sceneLevel3.CreateObject("sallySaltObj");
	auto& sallycmpt = sallySaltObj.AddComponent<PeterPepper>();
	auto& sallySpriteManager = sallySaltObj.AddComponent<SpriteManagerComponent>();
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Idle.png", 1, 1, 0, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Walking_Left.png", 4, 1, 16, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Walking_Right.png", 4, 1, 16, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Walking_Up.png", 4, 1, 16, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Walking_Down.png", 4, 1, 16, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Death.png", 6, 1, 2, 45, 45);
	sallySpriteManager.AddSprite("Charachters/Sally/Sally_Won.png", 2, 1, 2, 45, 45);

	auto& sallyCollider = sallySaltObj.AddComponent<RectColliderComponent>();
	sallyCollider.Init({ 0,1,45,44 }, 3, true);
	sallySaltObj.AddComponent<MovementComponent>();
	sallycmpt.SetSpawnLocation(298.f, 387.f);
	sallySaltObj.SetLocalPosition(298.f, 387.f);

	auto& sallyColliderSubje = sallyCollider.GetSubject();
	sallyColliderSubje.AddObserver(sallycmpt);
	auto& sallySaltSubje = sallycmpt.GetSubject();

	//the 3rd level manager will enable or disable the player(s) depending on the mode that was selected,
	//so by default its disabled
	peterPepperObj.SetEnabledState(false);
	sallySaltObj.SetEnabledState(false);
	pepperObj.SetEnabledState(false);

	levelManagercmpt.SetNormalPlayer(&peterPepperObj);
	levelManagercmpt.SetCoopPlayer(&sallySaltObj);
	levelManagercmpt.SetPepperObject(&pepperObj);
#pragma endregion

#pragma region Enemies
#pragma region EnemyPlayer
	auto& playerHotDogObj = sceneLevel3.CreateObject("playerHotDogObj");
	auto& playerHotDogCmpt = playerHotDogObj.AddComponent<MrHotDog>();
	playerHotDogCmpt.SetControlledByHuman(true);
	auto& playerHotDogSpriteManager = playerHotDogObj.AddComponent<SpriteManagerComponent>();
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Idle.png", 1, 1, 0, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Left.png", 2, 1, 8, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Right.png", 2, 1, 8, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Up.png", 2, 1, 8, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Down.png", 2, 1, 8, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Death.png", 4, 1, 10, 45, 45);
	playerHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Peppered.png", 2, 1, 8, 45, 45);
	playerHotDogCmpt.SetDeathDuration(4 / 10.f);//4frames / 10fps

	auto& playerHotDogCollider = playerHotDogObj.AddComponent<RectColliderComponent>();
	playerHotDogCollider.Init({ 0,1,45,44 }, 3, true);
	playerHotDogObj.AddComponent<MovementComponent>();
	playerHotDogCmpt.SetRespawnPosAndWalkDirection(30, 162, EnemyControllerComponent::MovementState::Right);
	auto& playerHotDogcollidersubje = playerHotDogCollider.GetSubject();
	playerHotDogcollidersubje.AddObserver(playerHotDogCmpt);
#pragma endregion
#pragma region Enemy1
	auto& mrHotDogObj1 = sceneLevel3.CreateObject("mrHotDogObj1");
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
	mrHotDogCollider1.Init({ 0,1,45,44 }, 3, true);
	mrHotDogObj1.AddComponent<MovementComponent>();
	auto& mrHotDogController1 = mrHotDogObj1.AddComponent<EnemyControllerComponent>();
	mrHotDogcmpt1.SetRespawnPosAndWalkDirection(30, 162, EnemyControllerComponent::MovementState::Right);
	auto& mrHotDogcollidersubje = mrHotDogCollider1.GetSubject();
	mrHotDogcollidersubje.AddObserver(mrHotDogcmpt1);
#pragma endregion

	//always set the first player, the 2nd player will be set or unset in the level manager once the leve becomes active
	mrHotDogController1.SetPlayer1(&peterPepperObj);

	//the 3nd level manager will enable or disable the enemies depending on the mode that was selected,
	//so by default its disabled
	playerHotDogObj.SetEnabledState(false);

	mrHotDogObj1.SetEnabledState(false);
	//mrHotDogObj2.SetEnabledState(false);
	//mrEggObj1.SetEnabledState(false);
	//mrPickleObj1.SetEnabledState(false);
	//mrPickleObj2.SetEnabledState(false);
	//mrPickleObj3.SetEnabledState(false);

	//register enemies to the levelmanager
	levelManagercmpt.SetEnemyPlayer(&playerHotDogObj);

	levelManagercmpt.AddEnemy(&mrHotDogObj1);
	//levelManagercmpt.AddEnemy(&mrHotDogObj2);
	//levelManagercmpt.AddEnemy(&mrEggObj1);
	//levelManagercmpt.AddEnemy(&mrPickleObj1);
	//levelManagercmpt.AddEnemy(&mrPickleObj2);
	//levelManagercmpt.AddEnemy(&mrPickleObj3);


	//let the enemies observe the player(s)
	petterPeppersubje.AddObserver(playerHotDogCmpt);

	petterPeppersubje.AddObserver(mrHotDogcmpt1);
	//petterPeppersubje.AddObserver(mrHotDogcmpt2);
	//petterPeppersubje.AddObserver(mrEggcmpt1);
	//petterPeppersubje.AddObserver(mrPicklecmpt1);
	//petterPeppersubje.AddObserver(mrPicklecmpt2);
	//petterPeppersubje.AddObserver(mrPicklecmpt3);

	sallySaltSubje.AddObserver(playerHotDogCmpt);

	sallySaltSubje.AddObserver(mrHotDogcmpt1);
	//sallySaltSubje.AddObserver(mrHotDogcmpt2);
	//sallySaltSubje.AddObserver(mrEggcmpt1);
	//sallySaltSubje.AddObserver(mrPicklecmpt1);
	//sallySaltSubje.AddObserver(mrPicklecmpt2);
	//sallySaltSubje.AddObserver(mrPicklecmpt3);
#pragma endregion

#pragma region LadderSetup
	auto& ladderObj1 = sceneLevel3.CreateObject("ladderObj1");
	ladderObj1.SetLocalPosition(51.f, 189.f);
	ladderObj1.AddComponent<LadderComponent>();
	auto& ladder1colliderCmpt = ladderObj1.AddComponent<RectColliderComponent>();
	ladder1colliderCmpt.Init({ 0, 0, 3, 152 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj2 = sceneLevel3.CreateObject("ladderObj2");
	ladderObj2.SetLocalPosition(51.f, 502.f);
	ladderObj2.AddComponent<LadderComponent>();
	auto& ladder2colliderCmpt = ladderObj2.AddComponent<RectColliderComponent>();
	ladder2colliderCmpt.Init({ 0, 0, 3, 107 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj3 = sceneLevel3.CreateObject("ladderObj3");
	ladderObj3.SetLocalPosition(117.f, 502.f);
	ladderObj3.AddComponent<LadderComponent>();
	auto& ladder3colliderCmpt = ladderObj3.AddComponent<RectColliderComponent>();
	ladder3colliderCmpt.Init({ 0, 0, 4, 107 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj4 = sceneLevel3.CreateObject("ladderObj4");
	ladderObj4.SetLocalPosition(184.f, 189.f);
	ladderObj4.AddComponent<LadderComponent>();
	auto& ladder4colliderCmpt = ladderObj4.AddComponent<RectColliderComponent>();
	ladder4colliderCmpt.Init({ 0, 0, 4, 107 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj5 = sceneLevel3.CreateObject("ladderObj5");
	ladderObj5.SetLocalPosition(184.f, 326.f);
	ladderObj5.AddComponent<LadderComponent>();
	auto& ladder5colliderCmpt = ladderObj5.AddComponent<RectColliderComponent>();
	ladder5colliderCmpt.Init({ 0, 0, 4, 372 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj6 = sceneLevel3.CreateObject("ladderObj6");
	ladderObj6.SetLocalPosition(251.f, 189.f);
	ladderObj6.AddComponent<LadderComponent>();
	auto& ladder6colliderCmpt = ladderObj6.AddComponent<RectColliderComponent>();
	ladder6colliderCmpt.Init({ 0, 0, 4, 62 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj7 = sceneLevel3.CreateObject("ladderObj7");
	ladderObj7.SetLocalPosition(251.f, 281.f);
	ladderObj7.AddComponent<LadderComponent>();
	auto& ladder7colliderCmpt = ladderObj7.AddComponent<RectColliderComponent>();
	ladder7colliderCmpt.Init({ 0, 0, 4, 149 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj8 = sceneLevel3.CreateObject("ladderObj8");
	ladderObj8.SetLocalPosition(251.f, 639.f);
	ladderObj8.AddComponent<LadderComponent>();
	auto& ladder8colliderCmpt = ladderObj8.AddComponent<RectColliderComponent>();
	ladder8colliderCmpt.Init({ 0, 0, 4, 59 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj9 = sceneLevel3.CreateObject("ladderObj9");
	ladderObj9.SetLocalPosition(318.f, 236.f);
	ladderObj9.AddComponent<LadderComponent>();
	auto& ladder9colliderCmpt = ladderObj9.AddComponent<RectColliderComponent>();
	ladder9colliderCmpt.Init({ 0, 0, 3, 195 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj10 = sceneLevel3.CreateObject("ladderObj10");
	ladderObj10.SetLocalPosition(318.f, 639.f);
	ladderObj10.AddComponent<LadderComponent>();
	auto& ladder10colliderCmpt = ladderObj10.AddComponent<RectColliderComponent>();
	ladder10colliderCmpt.Init({ 0, 0, 4, 59 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj11 = sceneLevel3.CreateObject("ladderObj11");
	ladderObj11.SetLocalPosition(385.f, 189.f);
	ladderObj11.AddComponent<LadderComponent>();
	auto& ladder11colliderCmpt = ladderObj11.AddComponent<RectColliderComponent>();
	ladder11colliderCmpt.Init({ 0, 0, 4, 62 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj12 = sceneLevel3.CreateObject("ladderObj12");
	ladderObj12.SetLocalPosition(385.f, 326.f);
	ladderObj12.AddComponent<LadderComponent>();
	auto& ladder12colliderCmpt = ladderObj12.AddComponent<RectColliderComponent>();
	ladder12colliderCmpt.Init({ 0, 0, 4, 59 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj13 = sceneLevel3.CreateObject("ladderObj13");
	ladderObj13.SetLocalPosition(385.f, 639.f);
	ladderObj13.AddComponent<LadderComponent>();
	auto& ladder13colliderCmpt = ladderObj13.AddComponent<RectColliderComponent>();
	ladder13colliderCmpt.Init({ 0, 0, 4, 59 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj14 = sceneLevel3.CreateObject("ladderObj14");
	ladderObj14.SetLocalPosition(452.f, 236.f);
	ladderObj14.AddComponent<LadderComponent>();
	auto& ladder14colliderCmpt = ladderObj14.AddComponent<RectColliderComponent>();
	ladder14colliderCmpt.Init({ 0, 0, 4, 105 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj15 = sceneLevel3.CreateObject("ladderObj15");
	ladderObj15.SetLocalPosition(452.f, 371.f);
	ladderObj15.AddComponent<LadderComponent>();
	auto& ladder15colliderCmpt = ladderObj15.AddComponent<RectColliderComponent>();
	ladder15colliderCmpt.Init({ 0, 0, 4, 327 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj16 = sceneLevel3.CreateObject("ladderObj16");
	ladderObj16.SetLocalPosition(519.f, 549.f);
	ladderObj16.AddComponent<LadderComponent>();
	auto& ladder16colliderCmpt = ladderObj16.AddComponent<RectColliderComponent>();
	ladder16colliderCmpt.Init({ 0, 0, 3, 59 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj17 = sceneLevel3.CreateObject("ladderObj17");
	ladderObj17.SetLocalPosition(586.f, 189.f);
	ladderObj17.AddComponent<LadderComponent>();
	auto& ladder17colliderCmpt = ladderObj17.AddComponent<RectColliderComponent>();
	ladder17colliderCmpt.Init({ 0, 0, 3, 152 }, 3, true, { 255, 255, 0, 128 });

	auto& ladderObj18 = sceneLevel3.CreateObject("ladderObj18");
	ladderObj18.SetLocalPosition(586.f, 502.f);
	ladderObj18.AddComponent<LadderComponent>();
	auto& ladder18colliderCmpt = ladderObj18.AddComponent<RectColliderComponent>();
	ladder18colliderCmpt.Init({ 0, 0, 3, 107 }, 3, true, { 255, 255, 0, 128 });
#pragma endregion

#pragma region WalkingPlatformSetup
	auto& walkPlatformObj1 = sceneLevel3.CreateObject("walkPlatformObj1");
	walkPlatformObj1.SetLocalPosition(30.f, 202.f);
	walkPlatformObj1.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform1colliderCmpt = walkPlatformObj1.AddComponent<RectColliderComponent>();
	walkPlatform1colliderCmpt.Init({ 0, 0, 580, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj2 = sceneLevel3.CreateObject("walkPlatformObj2");
	walkPlatformObj2.SetLocalPosition(30.f, 247);
	walkPlatformObj2.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform2colliderCmpt = walkPlatformObj2.AddComponent<RectColliderComponent>();
	walkPlatform2colliderCmpt.Init({ 0, 0, 580, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj3 = sceneLevel3.CreateObject("walkPlatformObj3");
	walkPlatformObj3.SetLocalPosition(30.f, 292.f);
	walkPlatformObj3.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform3colliderCmpt = walkPlatformObj3.AddComponent<RectColliderComponent>();
	walkPlatform3colliderCmpt.Init({ 0, 0, 310, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj4 = sceneLevel3.CreateObject("walkPlatformObj4");
	walkPlatformObj4.SetLocalPosition(432.f, 292.f);
	walkPlatformObj4.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform4colliderCmpt = walkPlatformObj4.AddComponent<RectColliderComponent>();
	walkPlatform4colliderCmpt.Init({ 0, 0, 176, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj5 = sceneLevel3.CreateObject("walkPlatformObj5");
	walkPlatformObj5.SetLocalPosition(30.f, 337.f);
	walkPlatformObj5.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform5colliderCmpt = walkPlatformObj5.AddComponent<RectColliderComponent>();
	walkPlatform5colliderCmpt.Init({ 0, 0, 580, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj6 = sceneLevel3.CreateObject("walkPlatformObj6");
	walkPlatformObj6.SetLocalPosition(298.f, 382.f);
	walkPlatformObj6.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform6colliderCmpt = walkPlatformObj6.AddComponent<RectColliderComponent>();
	walkPlatform6colliderCmpt.Init({ 0, 0, 176, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj7 = sceneLevel3.CreateObject("walkPlatformObj7");
	walkPlatformObj7.SetLocalPosition(165, 426.f);
	walkPlatformObj7.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform7colliderCmpt = walkPlatformObj7.AddComponent<RectColliderComponent>();
	walkPlatform7colliderCmpt.Init({ 0, 0, 310, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj8 = sceneLevel3.CreateObject("walkPlatformObj8");
	walkPlatformObj8.SetLocalPosition(30.f, 515.f);
	walkPlatformObj8.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform8colliderCmpt = walkPlatformObj8.AddComponent<RectColliderComponent>();
	walkPlatform8colliderCmpt.Init({ 0, 0, 175, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj9 = sceneLevel3.CreateObject("walkPlatformObj9");
	walkPlatformObj9.SetLocalPosition(432.f, 515.f);
	walkPlatformObj9.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform9colliderCmpt = walkPlatformObj9.AddComponent<RectColliderComponent>();
	walkPlatform9colliderCmpt.Init({ 0, 0, 175, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj10 = sceneLevel3.CreateObject("walkPlatformObj10");
	walkPlatformObj10.SetLocalPosition(30.f, 560.f);
	walkPlatformObj10.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform10colliderCmpt = walkPlatformObj10.AddComponent<RectColliderComponent>();
	walkPlatform10colliderCmpt.Init({ 0, 0, 175, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj11 = sceneLevel3.CreateObject("walkPlatformObj11");
	walkPlatformObj11.SetLocalPosition(432.f, 560.f);
	walkPlatformObj11.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform11colliderCmpt = walkPlatformObj11.AddComponent<RectColliderComponent>();
	walkPlatform11colliderCmpt.Init({ 0, 0, 175, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj12 = sceneLevel3.CreateObject("walkPlatformObj12");
	walkPlatformObj12.SetLocalPosition(30.f, 605.f);
	walkPlatformObj12.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform12colliderCmpt = walkPlatformObj12.AddComponent<RectColliderComponent>();
	walkPlatform12colliderCmpt.Init({ 0, 0, 175, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj13 = sceneLevel3.CreateObject("walkPlatformObj13");
	walkPlatformObj13.SetLocalPosition(432.f, 605.f);
	walkPlatformObj13.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform13colliderCmpt = walkPlatformObj13.AddComponent<RectColliderComponent>();
	walkPlatform13colliderCmpt.Init({ 0, 0, 175, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj14 = sceneLevel3.CreateObject("walkPlatformObj14");
	walkPlatformObj14.SetLocalPosition(165, 650.f);
	walkPlatformObj14.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform14colliderCmpt = walkPlatformObj14.AddComponent<RectColliderComponent>();
	walkPlatform14colliderCmpt.Init({ 0, 0, 310, 4 }, 3, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj15 = sceneLevel3.CreateObject("walkPlatformObj15");
	walkPlatformObj15.SetLocalPosition(165, 695.f);
	walkPlatformObj15.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform15colliderCmpt = walkPlatformObj15.AddComponent<RectColliderComponent>();
	walkPlatform15colliderCmpt.Init({ 0, 0, 310, 4 }, 3, true, { 0, 255, 0, 128 });
#pragma endregion

#pragma region IngredientsSetup
#pragma region burger1
	auto& bunTopObj1 = sceneLevel3.CreateObject("bunTopObj1");
	auto& bunTopCmpt1 = bunTopObj1.AddComponent<IngredientComponent>();
	auto& bunTopSubje1 = bunTopCmpt1.GetSubject();
	bunTopObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider1 = bunTopObj1.AddComponent<RectColliderComponent>();
	bunTopcollider1.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	bunTopObj1.SetLocalPosition(75.f, 190.f);
	bunTopCmpt1.SetSpawnLocation(75.f, 190.f);
	auto& bunTopColliderSubje1 = bunTopcollider1.GetSubject();
	bunTopColliderSubje1.AddObserver(bunTopCmpt1);

	auto& burgerMeatObj1 = sceneLevel3.CreateObject("burgerMeatObj1");
	auto& burgerMeatCmpt1 = burgerMeatObj1.AddComponent<IngredientComponent>();
	auto& burgerMeatSubje1 = burgerMeatCmpt1.GetSubject();
	burgerMeatObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider1 = burgerMeatObj1.AddComponent<RectColliderComponent>();
	burgerMeatcollider1.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	burgerMeatObj1.SetLocalPosition(75.f, 280.f);
	burgerMeatCmpt1.SetSpawnLocation(75.f, 280.f);
	auto& burgerMeatColliderSubje1 = burgerMeatcollider1.GetSubject();
	burgerMeatColliderSubje1.AddObserver(burgerMeatCmpt1);

	auto& bunBottomObj1 = sceneLevel3.CreateObject("bunBottomObj1");
	auto& bunBottomCmpt1 = bunBottomObj1.AddComponent<IngredientComponent>();
	auto& bunBottomSubje1 = bunBottomCmpt1.GetSubject();
	bunBottomObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider1 = bunBottomObj1.AddComponent<RectColliderComponent>();
	bunBottomcollider1.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	bunBottomObj1.SetLocalPosition(75.f, 325.f);
	bunBottomCmpt1.SetSpawnLocation(75.f, 325.f);
	auto& bunBottomColliderSubje1 = bunBottomcollider1.GetSubject();
	bunBottomColliderSubje1.AddObserver(bunBottomCmpt1);
#pragma endregion	
#pragma region burger2
	auto& bunTopObj2 = sceneLevel3.CreateObject("bunTopObj2");
	auto& bunTopCmpt2 = bunTopObj2.AddComponent<IngredientComponent>();
	auto& bunTopSubje2 = bunTopCmpt2.GetSubject();
	bunTopObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider2 = bunTopObj2.AddComponent<RectColliderComponent>();
	bunTopcollider2.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	bunTopObj2.SetLocalPosition(209.f, 190.f);
	bunTopCmpt2.SetSpawnLocation(209.f, 190.f);
	auto& bunTopColliderSubje2 = bunTopcollider2.GetSubject();
	bunTopColliderSubje2.AddObserver(bunTopCmpt2);

	auto& tomatoObj1 = sceneLevel3.CreateObject("tomatoObj1");
	auto& tomatoCmpt1 = tomatoObj1.AddComponent<IngredientComponent>();
	auto& tomatoSubje1 = tomatoCmpt1.GetSubject();
	tomatoObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Tomato.png", 88.f, 22.f);
	auto& tomatocollider1 = tomatoObj1.AddComponent<RectColliderComponent>();
	tomatocollider1.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	tomatoObj1.SetLocalPosition(209.f, 235.f);
	tomatoCmpt1.SetSpawnLocation(209.f, 235.f);
	auto& tomatoColliderSubje1 = tomatocollider1.GetSubject();
	tomatoColliderSubje1.AddObserver(tomatoCmpt1);

	auto& bunBottomObj2 = sceneLevel3.CreateObject("bunBottomObj2");
	auto& bunBottomCmpt2 = bunBottomObj2.AddComponent<IngredientComponent>();
	auto& bunBottomSubje2 = bunBottomCmpt2.GetSubject();
	bunBottomObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider2 = bunBottomObj2.AddComponent<RectColliderComponent>();
	bunBottomcollider2.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	bunBottomObj2.SetLocalPosition(209.f, 325.f);
	bunBottomCmpt2.SetSpawnLocation(209.f, 325.f);
	auto& bunBottomColliderSubje2 = bunBottomcollider2.GetSubject();
	bunBottomColliderSubje2.AddObserver(bunBottomCmpt2);
#pragma endregion
#pragma region burger3
	auto& bunTopObj3 = sceneLevel3.CreateObject("bunTopObj3");
	auto& bunTopCmpt3 = bunTopObj3.AddComponent<IngredientComponent>();
	auto& bunTopSubje3 = bunTopCmpt3.GetSubject();
	bunTopObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider3 = bunTopObj3.AddComponent<RectColliderComponent>();
	bunTopcollider3.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	bunTopObj3.SetLocalPosition(343.f, 190.f);
	bunTopCmpt3.SetSpawnLocation(343.f, 190.f);
	auto& bunTopColliderSubje3 = bunTopcollider3.GetSubject();
	bunTopColliderSubje3.AddObserver(bunTopCmpt3);

	auto& burgerMeatObj2 = sceneLevel3.CreateObject("burgerMeatObj2");
	auto& burgerMeatCmpt2 = burgerMeatObj2.AddComponent<IngredientComponent>();
	auto& burgerMeatSubje2 = burgerMeatCmpt2.GetSubject();
	burgerMeatObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider2 = burgerMeatObj2.AddComponent<RectColliderComponent>();
	burgerMeatcollider2.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	burgerMeatObj2.SetLocalPosition(343.f, 235.f);
	burgerMeatCmpt2.SetSpawnLocation(343.f, 235.f);
	auto& burgerMeatColliderSubje2 = burgerMeatcollider2.GetSubject();
	burgerMeatColliderSubje2.AddObserver(burgerMeatCmpt2);

	auto& bunBottomObj3 = sceneLevel3.CreateObject("bunBottomObj3");
	auto& bunBottomCmpt3 = bunBottomObj3.AddComponent<IngredientComponent>();
	auto& bunBottomSubje3 = bunBottomCmpt3.GetSubject();
	bunBottomObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider3 = bunBottomObj3.AddComponent<RectColliderComponent>();
	bunBottomcollider3.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	bunBottomObj3.SetLocalPosition(343.f, 325.f);
	bunBottomCmpt3.SetSpawnLocation(343.f, 325.f);
	auto& bunBottomColliderSubje3 = bunBottomcollider3.GetSubject();
	bunBottomColliderSubje3.AddObserver(bunBottomCmpt3);
#pragma endregion
#pragma region burger4
	auto& bunTopObj4 = sceneLevel3.CreateObject("bunTopObj4");
	auto& bunTopCmpt4 = bunTopObj4.AddComponent<IngredientComponent>();
	auto& bunTopSubje4 = bunTopCmpt4.GetSubject();
	bunTopObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider4 = bunTopObj4.AddComponent<RectColliderComponent>();
	bunTopcollider4.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	bunTopObj4.SetLocalPosition(477.f, 190.f);
	bunTopCmpt4.SetSpawnLocation(477.f, 190.f);
	auto& bunTopColliderSubje4 = bunTopcollider4.GetSubject();
	bunTopColliderSubje4.AddObserver(bunTopCmpt4);

	auto& tomatoObj2 = sceneLevel3.CreateObject("tomatoObj2");
	auto& tomatoCmpt2 = tomatoObj2.AddComponent<IngredientComponent>();
	auto& tomatoSubje2 = tomatoCmpt2.GetSubject();
	tomatoObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Tomato.png", 88.f, 22.f);
	auto& tomatocollider2 = tomatoObj2.AddComponent<RectColliderComponent>();
	tomatocollider2.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	tomatoObj2.SetLocalPosition(477.f, 280.f);
	tomatoCmpt2.SetSpawnLocation(477.f, 280.f);
	auto& tomatoColliderSubje2 = tomatocollider2.GetSubject();
	tomatoColliderSubje2.AddObserver(tomatoCmpt2);

	auto& bunBottomObj4 = sceneLevel3.CreateObject("bunBottomObj4");
	auto& bunBottomCmpt4 = bunBottomObj4.AddComponent<IngredientComponent>();
	auto& bunBottomSubje4 = bunBottomCmpt4.GetSubject();
	bunBottomObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider4 = bunBottomObj4.AddComponent<RectColliderComponent>();
	bunBottomcollider4.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	bunBottomObj4.SetLocalPosition(477.f, 325.f);
	bunBottomCmpt4.SetSpawnLocation(477.f, 325.f);
	auto& bunBottomColliderSubje4 = bunBottomcollider4.GetSubject();
	bunBottomColliderSubje4.AddObserver(bunBottomCmpt4);
#pragma endregion
#pragma region burger5
	auto& bunTopObj5 = sceneLevel3.CreateObject("bunTopObj5");
	auto& bunTopCmpt5 = bunTopObj5.AddComponent<IngredientComponent>();
	auto& bunTopSubje5 = bunTopCmpt5.GetSubject();
	bunTopObj5.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider5 = bunTopObj5.AddComponent<RectColliderComponent>();
	bunTopcollider5.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	bunTopObj5.SetLocalPosition(75.f, 500.f);
	bunTopCmpt5.SetSpawnLocation(75.f, 500.f);
	auto& bunTopColliderSubje5 = bunTopcollider5.GetSubject();
	bunTopColliderSubje5.AddObserver(bunTopCmpt5);

	auto& tomatoObj3 = sceneLevel3.CreateObject("tomatoObj3");
	auto& tomatoCmpt3 = tomatoObj3.AddComponent<IngredientComponent>();
	auto& tomatoSubje3 = tomatoCmpt3.GetSubject();
	tomatoObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Tomato.png", 88.f, 22.f);
	auto& tomatocollider3 = tomatoObj3.AddComponent<RectColliderComponent>();
	tomatocollider3.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	tomatoObj3.SetLocalPosition(75.f, 545.f);
	tomatoCmpt3.SetSpawnLocation(75.f, 545.f);
	auto& tomatoColliderSubje3 = tomatocollider3.GetSubject();
	tomatoColliderSubje3.AddObserver(tomatoCmpt3);

	auto& bunBottomObj5 = sceneLevel3.CreateObject("bunBottomObj5");
	auto& bunBottomCmpt5 = bunBottomObj5.AddComponent<IngredientComponent>();
	auto& bunBottomSubje5 = bunBottomCmpt5.GetSubject();
	bunBottomObj5.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider5 = bunBottomObj5.AddComponent<RectColliderComponent>();
	bunBottomcollider5.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	bunBottomObj5.SetLocalPosition(75.f, 590.f);
	bunBottomCmpt5.SetSpawnLocation(75.f, 590.f);
	auto& bunBottomColliderSubje5 = bunBottomcollider5.GetSubject();
	bunBottomColliderSubje5.AddObserver(bunBottomCmpt5);
#pragma endregion
#pragma region burger6
	auto& bunTopObj6 = sceneLevel3.CreateObject("bunTopObj6");
	auto& bunTopCmpt6 = bunTopObj6.AddComponent<IngredientComponent>();
	auto& bunTopSubje6 = bunTopCmpt6.GetSubject();
	bunTopObj6.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider6 = bunTopObj6.AddComponent<RectColliderComponent>();
	bunTopcollider6.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	bunTopObj6.SetLocalPosition(477.f, 500.f);
	bunTopCmpt6.SetSpawnLocation(477.f, 500.f);
	auto& bunTopColliderSubje6 = bunTopcollider6.GetSubject();
	bunTopColliderSubje6.AddObserver(bunTopCmpt6);

	auto& burgerMeatObj3 = sceneLevel3.CreateObject("burgerMeatObj3");
	auto& burgerMeatCmpt3 = burgerMeatObj3.AddComponent<IngredientComponent>();
	auto& burgerMeatSubje3 = burgerMeatCmpt3.GetSubject();
	burgerMeatObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider3 = burgerMeatObj3.AddComponent<RectColliderComponent>();
	burgerMeatcollider3.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	burgerMeatObj3.SetLocalPosition(477.f, 545.f);
	burgerMeatCmpt3.SetSpawnLocation(477.f, 545.f);
	auto& burgerMeatColliderSubje3 = burgerMeatcollider3.GetSubject();
	burgerMeatColliderSubje3.AddObserver(burgerMeatCmpt3);

	auto& bunBottomObj6 = sceneLevel3.CreateObject("bunBottomObj6");
	auto& bunBottomCmpt6 = bunBottomObj6.AddComponent<IngredientComponent>();
	auto& bunBottomSubje6 = bunBottomCmpt6.GetSubject();
	bunBottomObj6.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider6 = bunBottomObj6.AddComponent<RectColliderComponent>();
	bunBottomcollider6.Init({ 2, 0, 84, 20 }, 3, true, { 133, 0, 133, 255 });
	bunBottomObj6.SetLocalPosition(477.f, 590.f);
	bunBottomCmpt6.SetSpawnLocation(477.f, 590.f);
	auto& bunBottomColliderSubje6 = bunBottomcollider6.GetSubject();
	bunBottomColliderSubje6.AddObserver(bunBottomCmpt6);
#pragma endregion
#pragma endregion

#pragma region PlateSetup
	auto& plateObj1 = sceneLevel3.CreateObject("plateObj1");
	auto& plateCmpt1 = plateObj1.AddComponent<PlateComponent>();
	plateCmpt1.SetIngredientFullCount(3);
	auto& plateSubje1 = plateCmpt1.GetSubject();
	plateObj1.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider1 = plateObj1.AddComponent<RectColliderComponent>();
	plateCollider1.Init({ 0, 8, 104, 4 }, 3, true, { 0, 133, 133, 255 });
	plateObj1.SetLocalPosition(68.f, 450.f);
	auto& plateColliderSubje1 = plateCollider1.GetSubject();
	plateColliderSubje1.AddObserver(plateCmpt1);

	auto& plateObj2 = sceneLevel3.CreateObject("plateObj2");
	auto& plateCmpt2 = plateObj2.AddComponent<PlateComponent>();
	plateCmpt2.SetIngredientFullCount(3);
	auto& plateSubje2 = plateCmpt2.GetSubject();
	plateObj2.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider2 = plateObj2.AddComponent<RectColliderComponent>();
	plateCollider2.Init({ 0, 8, 104, 4 }, 3, true, { 0, 133, 133, 255 });
	plateObj2.SetLocalPosition(68.f, 750.f);
	auto& plateColliderSubje2 = plateCollider2.GetSubject();
	plateColliderSubje2.AddObserver(plateCmpt2);

	auto& plateObj3 = sceneLevel3.CreateObject("plateObj3");
	auto& plateCmpt3 = plateObj3.AddComponent<PlateComponent>();
	plateCmpt3.SetIngredientFullCount(3);
	auto& plateSubje3 = plateCmpt3.GetSubject();
	plateObj3.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider3 = plateObj3.AddComponent<RectColliderComponent>();
	plateCollider3.Init({ 0, 8, 104, 4 }, 3, true, { 0, 133, 133, 255 });
	plateObj3.SetLocalPosition(202.f, 525.f);
	auto& plateColliderSubje3 = plateCollider3.GetSubject();
	plateColliderSubje3.AddObserver(plateCmpt3);

	auto& plateObj4 = sceneLevel3.CreateObject("plateObj4");
	auto& plateCmpt4 = plateObj4.AddComponent<PlateComponent>();
	plateCmpt4.SetIngredientFullCount(3);
	auto& plateSubje4 = plateCmpt4.GetSubject();
	plateObj4.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider4 = plateObj4.AddComponent<RectColliderComponent>();
	plateCollider4.Init({ 0, 8, 104, 4 }, 3, true, { 0, 133, 133, 255 });
	plateObj4.SetLocalPosition(336.f, 525.f);
	auto& plateColliderSubje4 = plateCollider4.GetSubject();
	plateColliderSubje4.AddObserver(plateCmpt4);

	auto& plateObj5 = sceneLevel3.CreateObject("plateObj5");
	auto& plateCmpt5 = plateObj5.AddComponent<PlateComponent>();
	plateCmpt5.SetIngredientFullCount(3);
	auto& plateSubje5 = plateCmpt5.GetSubject();
	plateObj5.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider5 = plateObj5.AddComponent<RectColliderComponent>();
	plateCollider5.Init({ 0, 8, 104, 4 }, 3, true, { 0, 133, 133, 255 });
	plateObj5.SetLocalPosition(470.f, 450.f);
	auto& plateColliderSubje5 = plateCollider5.GetSubject();
	plateColliderSubje5.AddObserver(plateCmpt5);
	//6
	auto& plateObj6 = sceneLevel3.CreateObject("plateObj6");
	auto& plateCmpt6 = plateObj6.AddComponent<PlateComponent>();
	plateCmpt6.SetIngredientFullCount(3);
	auto& plateSubje6 = plateCmpt6.GetSubject();
	plateObj6.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider6 = plateObj6.AddComponent<RectColliderComponent>();
	plateCollider6.Init({ 0, 8, 104, 4 }, 3, true, { 0, 133, 133, 255 });
	plateObj6.SetLocalPosition(470.f, 750.f);
	auto& plateColliderSubje6 = plateCollider6.GetSubject();
	plateColliderSubje6.AddObserver(plateCmpt6);
#pragma endregion

#pragma region UI/SCORE
	const int uiFontSize{ 20 };
	auto uiFont = ResourceManager::GetInstance().LoadFont("Fonts/BurgerTime_Font.otf", uiFontSize);

	//
	//Static UI
	//
#pragma region Static UI
	auto& oneUpTextObject = sceneLevel3.CreateObject("oneUpTextObject");
	oneUpTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 255, 0, 0 }, "1UP");
	oneUpTextObject.SetLocalPosition(float(uiFontSize * 4), 5);

	auto& hiScoreTextObject = sceneLevel3.CreateObject("hiScoreTextObject");
	hiScoreTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 255, 0, 0 }, "HI-SCORE");
	hiScoreTextObject.SetLocalPosition(float(uiFontSize * 4) + (uiFontSize * 5), 5);

	auto& pepperTextObject = sceneLevel3.CreateObject("pepperTextObject");
	pepperTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 0, 255, 0 }, "PEPPER");
	pepperTextObject.SetLocalPosition(float(windowW - (uiFontSize * 7)), 5);
#pragma endregion

	//
	//Dynamic UI
	//
	auto& peterLivesObj = sceneLevel3.CreateObject("peterLivesObj");
	auto& livescmpt = peterLivesObj.AddComponent<LivesComponent>();
	livescmpt.SetTexture("UI/Peter_Life_Icon.png", 3.25f, 3.25f);
	peterLivesObj.SetLocalPosition(10.f, windowH - 40.f);
	auto& livessubje = livescmpt.GetSubject();
	livessubje.AddObserver(levelManagercmpt);
	petterPeppersubje.AddObserver(livescmpt);
	sallySaltSubje.AddObserver(livescmpt);


	auto& scoreObject = sceneLevel3.CreateObject("scoreObject");
	auto& scoretextcomp = scoreObject.AddComponent<TextComponent>();
	scoretextcomp.Setup(uiFont);
	auto& scoreCmpt = scoreObject.AddComponent<ScoreComponent>();
	scoreCmpt.SetTextComponent(scoretextcomp);
	scoreObject.SetLocalPosition(uiFontSize * 7.f, uiFontSize * 1.5f);
	auto& scoresubje = scoreCmpt.GetSubject();
	scoresubje.AddObserver(livescmpt);
	levelManagercmpt.SetScoreComponent(&scoreCmpt);

#pragma region ScoreObservingIngredients
	bunBottomSubje1.AddObserver(scoreCmpt);
	bunBottomSubje2.AddObserver(scoreCmpt);
	bunBottomSubje3.AddObserver(scoreCmpt);
	bunBottomSubje4.AddObserver(scoreCmpt);
	bunBottomSubje5.AddObserver(scoreCmpt);
	bunBottomSubje6.AddObserver(scoreCmpt);
	burgerMeatSubje1.AddObserver(scoreCmpt);
	tomatoSubje1.AddObserver(scoreCmpt);
	burgerMeatSubje2.AddObserver(scoreCmpt);
	tomatoSubje2.AddObserver(scoreCmpt);
	tomatoSubje3.AddObserver(scoreCmpt);
	burgerMeatSubje3.AddObserver(scoreCmpt);
	bunTopSubje1.AddObserver(scoreCmpt);
	bunTopSubje2.AddObserver(scoreCmpt);
	bunTopSubje3.AddObserver(scoreCmpt);
	bunTopSubje4.AddObserver(scoreCmpt);
	bunTopSubje5.AddObserver(scoreCmpt);
	bunTopSubje6.AddObserver(scoreCmpt);
#pragma endregion

	//TODO: hi score component should read hi-score from file and observe the score object,
	//if the score components value is bigger than the current high score update it

	auto& hiScoreObject = sceneLevel3.CreateObject("scoreObject");
	auto& hiScoretextcomp = hiScoreObject.AddComponent<TextComponent>();
	hiScoretextcomp.Setup(uiFont);
	auto& hiScoreCmpt = hiScoreObject.AddComponent<HiScoreComponent>();
	hiScoreCmpt.SetTextComponent(hiScoretextcomp);
	hiScoreCmpt.SetScoreComponent(scoreCmpt);
	hiScoreObject.SetLocalPosition(uiFontSize * 15.f, uiFontSize * 1.5f);

	auto& pepperCountObject = sceneLevel3.CreateObject("pepperCountObject");
	auto& pepperCountTextComp = pepperCountObject.AddComponent<TextComponent>();
	pepperCountTextComp.Setup(uiFont);
	auto& pepperCountCmpt = pepperCountObject.AddComponent<PepperCountComponent>();
	pepperCountCmpt.SetTextComponent(pepperCountTextComp);
	pepperCountObject.SetLocalPosition(windowW - (uiFontSize * 2.f), uiFontSize * 1.5f);
#pragma endregion

#pragma region InputCommandsPlayer
	//peter pepper input
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
	throwPepperCmdController1.SetPepperCountComponent(&pepperCountCmpt);
	auto& throwPepperCmdKeyBoard1 = inputmanager.AddCommand<ThrowPepperCommand>(InputManager::KeyboardKey::Key_CTRL_L, InputManager::ButtonPressState::OnPressed);
	throwPepperCmdKeyBoard1.SetPlayer(&peterPepperObj);
	throwPepperCmdKeyBoard1.SetPepper(&pepperObj);
	throwPepperCmdKeyBoard1.SetPepperCountComponent(&pepperCountCmpt);


	//sally salt input
	inputmanager.AddCommand<WalkLeftCommand>(PCController::ControllerButton::Button_DPAD_LEFT, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkLeftCommand>(InputManager::KeyboardKey::Key_ARROW_LEFT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkRightCommand>(PCController::ControllerButton::Button_DPAD_RIGHT, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkRightCommand>(InputManager::KeyboardKey::Key_ARROW_RIGHT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkUpCommand>(PCController::ControllerButton::Button_DPAD_UP, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkUpCommand>(InputManager::KeyboardKey::Key_ARROW_UP, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkDownCommand>(PCController::ControllerButton::Button_DPAD_DOWN, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&sallySaltObj);
	inputmanager.AddCommand<WalkDownCommand>(InputManager::KeyboardKey::Key_ARROW_DOWN, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&sallySaltObj);

	auto& throwPepperCmdController2 = inputmanager.AddCommand<ThrowPepperCommand>(PCController::ControllerButton::Button_Square, InputManager::ButtonPressState::OnPressed, 1);
	throwPepperCmdController2.SetPlayer(&sallySaltObj);
	throwPepperCmdController2.SetPepper(&pepperObj);
	throwPepperCmdController2.SetPepperCountComponent(&pepperCountCmpt);

	auto& throwPepperCmdKeyBoard2 = inputmanager.AddCommand<ThrowPepperCommand>(InputManager::KeyboardKey::Key_CTRL_R, InputManager::ButtonPressState::OnPressed);
	throwPepperCmdKeyBoard2.SetPlayer(&sallySaltObj);
	throwPepperCmdKeyBoard2.SetPepper(&pepperObj);
	throwPepperCmdKeyBoard2.SetPepperCountComponent(&pepperCountCmpt);

	//player Hotdog input
	inputmanager.AddCommand<WalkLeftCommand>(PCController::ControllerButton::Button_DPAD_LEFT, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkLeftCommand>(InputManager::KeyboardKey::Key_ARROW_LEFT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkRightCommand>(PCController::ControllerButton::Button_DPAD_RIGHT, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkRightCommand>(InputManager::KeyboardKey::Key_ARROW_RIGHT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkUpCommand>(PCController::ControllerButton::Button_DPAD_UP, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkUpCommand>(InputManager::KeyboardKey::Key_ARROW_UP, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkDownCommand>(PCController::ControllerButton::Button_DPAD_DOWN, InputManager::ButtonPressState::PressedContinuous, 1).SetPlayer(&playerHotDogObj);
	inputmanager.AddCommand<WalkDownCommand>(InputManager::KeyboardKey::Key_ARROW_DOWN, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&playerHotDogObj);
#pragma endregion

	auto& levelManagersubje = levelManagercmpt.GetSubject();
	levelManagersubje.AddObserver(petercmpt);
	levelManagersubje.AddObserver(sallycmpt);

	levelManagersubje.AddObserver(mrHotDogcmpt1);
	//levelManagersubje.AddObserver(mrHotDogcmpt2);
	//levelManagersubje.AddObserver(mrHotDogcmpt3);
	//levelManagersubje.AddObserver(mrEggcmpt1);

	plateSubje1.AddObserver(levelManagercmpt);
	plateSubje2.AddObserver(levelManagercmpt);
	plateSubje3.AddObserver(levelManagercmpt);
	plateSubje4.AddObserver(levelManagercmpt);
	plateSubje5.AddObserver(levelManagercmpt);
	plateSubje6.AddObserver(levelManagercmpt);



	sceneLevel3.LateInit();//intialize the new scene
}
