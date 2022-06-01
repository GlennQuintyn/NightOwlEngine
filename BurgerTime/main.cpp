#include "BurgerTimePCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "NightOwlEngine.h"

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

//void InitializeScenesAndObject();

using namespace dae;

int main(int, char* [])
{
	auto engine = new dae::NightOwlEngine(640, 900, "BURGERTIME");

	engine->Initialize();

	// tell the resource manager where it can find the game data
	ResourceManager::GetInstance().Init("../Data/");


#pragma region LoadGame

	//std::cout << "\n\n-player 1:\n\tButton Y: Lose a life\n\tButton R1: add 25 to score\n\tButton R3: add 50 to score\n\n";
	//std::cout << "-player 2:\n\tButton X: Lose a life\n\tButton L1: add 25 to score\n\tButton L3: add 50 to score\n\n";

	//auto& sceneModeSelect = SceneManager::GetInstance().CreateScene("ModeSelect");
	auto& sceneLevel1 = SceneManager::GetInstance().CreateScene("Level1");
	//auto& sceneLevel2 = SceneManager::GetInstance().CreateScene("Level2");
	//auto& sceneLevel3 = SceneManager::GetInstance().CreateScene("Level2");
	auto& inputmanager = InputManager::GetInstance();

	auto fpsFont = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 19);
	//auto normalfont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	int windowW{}, windowH{};
	SDL_GetWindowSize(engine->GetWindow(), &windowW, &windowH);

	auto& backgroundObject = sceneLevel1.CreateObject("backgroundObject");
	backgroundObject.AddComponent<Texture2DComponent>().SetTexture("Level/Level1.png");

	auto& fpsCounterObj = sceneLevel1.CreateObject("fpsCounterObj");
	auto& fpstextComponent = fpsCounterObj.AddComponent<TextComponent>();
	auto& fpsComponent = fpsCounterObj.AddComponent<FPSComponent>();
	fpsComponent.SetTextComponent(fpstextComponent);
	fpsCounterObj.SetLocalPosition(5.f, 5.f);
	fpstextComponent.SetFont(fpsFont);
	fpstextComponent.SetTextColor({ 0, 255, 255 });


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
	//pepperObj.SetLocalPosition(300.f, 300.f);
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

	auto& peterCollider = peterPepperObj.AddComponent<RectColliderComponent>();
	peterCollider.Init({ 0,0,45,45 }, -1, true);
	peterPepperObj.AddComponent<MovementComponent>();
	petercmpt.SetSpawnLocation(298.f, 563.f);
	peterPepperObj.SetLocalPosition(298.f, 563.f);

	auto& pettercollidersubje = peterCollider.GetSubject();
	pettercollidersubje.AddObserver(petercmpt);

	auto& mrHotDogObj = sceneLevel1.CreateObject("mrHotDogObj");
	auto& mrHotDogcmpt = mrHotDogObj.AddComponent<MrHotDog>();
	auto& mrHotDogSpriteManager = mrHotDogObj.AddComponent<SpriteManagerComponent>();
	mrHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Idle.png", 1, 1, 0, 45, 45);
	mrHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Left.png", 2, 1, 8, 45, 45);
	mrHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Right.png", 2, 1, 8, 45, 45);
	mrHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Up.png", 2, 1, 8, 45, 45);
	mrHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Walking_Down.png", 2, 1, 8, 45, 45);
	mrHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Death.png", 4, 1, 10, 45, 45);
	mrHotDogSpriteManager.AddSprite("Charachters/MrHotDog/MrHotDog_Peppered.png", 2, 1, 8, 45, 45);

	auto& mrHotDogCollider = mrHotDogObj.AddComponent<RectColliderComponent>();
	mrHotDogCollider.Init({ 0,0,45,45 }, -2, true);
	mrHotDogObj.AddComponent<MovementComponent>();
	auto& mrHotDogController = mrHotDogObj.AddComponent<EnemyControllerComponent>();
	mrHotDogController.SetPlayer1(&peterPepperObj);
	//mrHotDogObj.SetLocalPosition(29, 162);
	mrHotDogObj.SetLocalPosition(97, 320);

	auto& mrHotDogcollidersubje = mrHotDogCollider.GetSubject();
	mrHotDogcollidersubje.AddObserver(mrHotDogcmpt);

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
	ladderObj9.SetLocalPosition(519.f, 338.f);
	ladderObj9.AddComponent<LadderComponent>();
	auto& ladder9colliderCmpt = ladderObj9.AddComponent<RectColliderComponent>();
	ladder9colliderCmpt.Init({ 0, 0, 3, 269 }, 18, true, { 255, 255, 0, 128 });

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
	auto& bunBottomcmpt1 = bunBottomObj1.AddComponent<IngredientComponent>();
	bunBottomObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider1 = bunBottomObj1.AddComponent<RectColliderComponent>();
	bunBottomcollider1.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunBottomObj1.SetLocalPosition(75.f, 595.f);
	auto& bunBottomSubje1 = bunBottomcollider1.GetSubject();
	bunBottomSubje1.AddObserver(bunBottomcmpt1);

	auto& burgerMeatObj1 = sceneLevel1.CreateObject("burgerMeatObj1");
	auto& burgerBottomcmpt1 = burgerMeatObj1.AddComponent<IngredientComponent>();
	burgerMeatObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider1 = burgerMeatObj1.AddComponent<RectColliderComponent>();
	burgerMeatcollider1.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	burgerMeatObj1.SetLocalPosition(75.f, 505.f);
	auto& burgerMeatSubje1 = burgerMeatcollider1.GetSubject();
	burgerMeatSubje1.AddObserver(burgerBottomcmpt1);

	auto& lettuceObj1 = sceneLevel1.CreateObject("lettuceObj1");
	auto& lettucecmpt1 = lettuceObj1.AddComponent<IngredientComponent>();
	lettuceObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider1 = lettuceObj1.AddComponent<RectColliderComponent>();
	lettucecollider1.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	lettuceObj1.SetLocalPosition(75.f, 375.f);
	auto& lettuceSubje1 = lettucecollider1.GetSubject();
	lettuceSubje1.AddObserver(lettucecmpt1);

	auto& bunTopObj1 = sceneLevel1.CreateObject("bunTopObj1");
	auto& bunTopcmpt1 = bunTopObj1.AddComponent<IngredientComponent>();
	bunTopObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider1 = bunTopObj1.AddComponent<RectColliderComponent>();
	bunTopcollider1.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunTopObj1.SetLocalPosition(75.f, 285.f);
	auto& bunTopSubje1 = bunTopcollider1.GetSubject();
	bunTopSubje1.AddObserver(bunTopcmpt1);
#pragma endregion
#pragma region burger2
	auto& bunBottomObj2 = sceneLevel1.CreateObject("bunBottomObj2");
	auto& bunBottomcmpt2 = bunBottomObj2.AddComponent<IngredientComponent>();
	bunBottomObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider2 = bunBottomObj2.AddComponent<RectColliderComponent>();
	bunBottomcollider2.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunBottomObj2.SetLocalPosition(209.f, 595.f);
	auto& bunBottomSubje2 = bunBottomcollider2.GetSubject();
	bunBottomSubje2.AddObserver(bunBottomcmpt2);

	auto& burgerMeatObj2 = sceneLevel1.CreateObject("burgerMeatObj2");
	auto& burgerBottomcmpt2 = burgerMeatObj2.AddComponent<IngredientComponent>();
	burgerMeatObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider2 = burgerMeatObj2.AddComponent<RectColliderComponent>();
	burgerMeatcollider2.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	burgerMeatObj2.SetLocalPosition(209.f, 505.f);
	auto& burgerMeatSubje2 = burgerMeatcollider2.GetSubject();
	burgerMeatSubje2.AddObserver(burgerBottomcmpt2);

	auto& lettuceObj2 = sceneLevel1.CreateObject("lettuceObj2");
	auto& lettucecmpt2 = lettuceObj2.AddComponent<IngredientComponent>();
	lettuceObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider2 = lettuceObj2.AddComponent<RectColliderComponent>();
	lettucecollider2.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	lettuceObj2.SetLocalPosition(209.f, 415.f);
	auto& lettuceSubje2 = lettucecollider2.GetSubject();
	lettuceSubje2.AddObserver(lettucecmpt2);

	auto& bunTopObj2 = sceneLevel1.CreateObject("bunTopObj2");
	auto& bunTopcmpt2 = bunTopObj2.AddComponent<IngredientComponent>();
	bunTopObj2.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider2 = bunTopObj2.AddComponent<RectColliderComponent>();
	bunTopcollider2.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunTopObj2.SetLocalPosition(209.f, 195.f);
	auto& bunTopSubje2 = bunTopcollider2.GetSubject();
	bunTopSubje2.AddObserver(bunTopcmpt2);
#pragma endregion
#pragma region burger3
	auto& bunBottomObj3 = sceneLevel1.CreateObject("bunBottomObj3");
	auto& bunBottomcmpt3 = bunBottomObj3.AddComponent<IngredientComponent>();
	bunBottomObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider3 = bunBottomObj3.AddComponent<RectColliderComponent>();
	bunBottomcollider3.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunBottomObj3.SetLocalPosition(343.f, 595.f);
	auto& bunBottomSubje3 = bunBottomcollider3.GetSubject();
	bunBottomSubje3.AddObserver(bunBottomcmpt3);

	auto& burgerMeatObj3 = sceneLevel1.CreateObject("burgerMeatObj3");
	auto& burgerBottomcmpt3 = burgerMeatObj3.AddComponent<IngredientComponent>();
	burgerMeatObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider3 = burgerMeatObj3.AddComponent<RectColliderComponent>();
	burgerMeatcollider3.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	burgerMeatObj3.SetLocalPosition(343.f, 415.f);
	auto& burgerMeatSubje3 = burgerMeatcollider3.GetSubject();
	burgerMeatSubje3.AddObserver(burgerBottomcmpt3);

	auto& lettuceObj3 = sceneLevel1.CreateObject("lettuceObj3");
	auto& lettucecmpt3 = lettuceObj3.AddComponent<IngredientComponent>();
	lettuceObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider3 = lettuceObj3.AddComponent<RectColliderComponent>();
	lettucecollider3.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	lettuceObj3.SetLocalPosition(343.f, 285.f);
	auto& lettuceSubje3 = lettucecollider3.GetSubject();
	lettuceSubje3.AddObserver(lettucecmpt3);

	auto& bunTopObj3 = sceneLevel1.CreateObject("bunTopObj3");
	auto& bunTopcmpt3 = bunTopObj3.AddComponent<IngredientComponent>();
	bunTopObj3.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider3 = bunTopObj3.AddComponent<RectColliderComponent>();
	bunTopcollider3.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunTopObj3.SetLocalPosition(343.f, 195.f);
	auto& bunTopSubje3 = bunTopcollider3.GetSubject();
	bunTopSubje3.AddObserver(bunTopcmpt3);
#pragma endregion
#pragma region burger4
	auto& bunBottomObj4 = sceneLevel1.CreateObject("bunBottomObj4");
	auto& bunBottomcmpt4 = bunBottomObj4.AddComponent<IngredientComponent>();
	bunBottomObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider4 = bunBottomObj4.AddComponent<RectColliderComponent>();
	bunBottomcollider4.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunBottomObj4.SetLocalPosition(477.f, 460.f);
	auto& bunBottomSubje4 = bunBottomcollider4.GetSubject();
	bunBottomSubje4.AddObserver(bunBottomcmpt4);

	auto& burgerMeatObj4 = sceneLevel1.CreateObject("burgerMeatObj4");
	auto& burgerBottomcmpt4 = burgerMeatObj4.AddComponent<IngredientComponent>();
	burgerMeatObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider4 = burgerMeatObj4.AddComponent<RectColliderComponent>();
	burgerMeatcollider4.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	burgerMeatObj4.SetLocalPosition(477.f, 375.f);
	auto& burgerMeatSubje4 = burgerMeatcollider4.GetSubject();
	burgerMeatSubje4.AddObserver(burgerBottomcmpt4);

	auto& lettuceObj4 = sceneLevel1.CreateObject("lettuceObj4");
	auto& lettucecmpt4 = lettuceObj4.AddComponent<IngredientComponent>();
	lettuceObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider4 = lettuceObj4.AddComponent<RectColliderComponent>();
	lettucecollider4.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	lettuceObj4.SetLocalPosition(477.f, 285.f);
	auto& lettuceSubje4 = lettucecollider4.GetSubject();
	lettuceSubje4.AddObserver(lettucecmpt4);

	auto& bunTopObj4 = sceneLevel1.CreateObject("bunTopObj4");
	auto& bunTopcmpt4 = bunTopObj4.AddComponent<IngredientComponent>();
	bunTopObj4.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider4 = bunTopObj4.AddComponent<RectColliderComponent>();
	bunTopcollider4.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunTopObj4.SetLocalPosition(477.f, 195.f);
	auto& bunTopSubje4 = bunTopcollider4.GetSubject();
	bunTopSubje4.AddObserver(bunTopcmpt4);
#pragma endregion
#pragma endregion

#pragma region PlateSetup
	auto& plateObj1 = sceneLevel1.CreateObject("plateObj1");
	auto& plateCmpt1 = plateObj1.AddComponent<PlateComponent>();
	plateObj1.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider1 = plateObj1.AddComponent<RectColliderComponent>();
	plateCollider1.Init({ 0, 8, 104, 4 }, 100, true, { 0, 133, 133, 255 });
	//plateObj1.SetLocalPosition(67.f, 690.f);
	plateObj1.SetLocalPosition(68.f, 800.f);
	auto& plateSubje1 = plateCollider1.GetSubject();
	plateSubje1.AddObserver(plateCmpt1);

	auto& plateObj2 = sceneLevel1.CreateObject("plateObj2");
	auto& plateCmpt2 = plateObj2.AddComponent<PlateComponent>();
	plateObj2.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider2 = plateObj2.AddComponent<RectColliderComponent>();
	plateCollider2.Init({ 0, 8, 104, 4 }, 100, true, { 0, 133, 133, 255 });
	plateObj2.SetLocalPosition(202.f, 800.f);
	auto& plateSubje2 = plateCollider2.GetSubject();
	plateSubje2.AddObserver(plateCmpt2);

	auto& plateObj3 = sceneLevel1.CreateObject("plateObj3");
	auto& plateCmpt3 = plateObj3.AddComponent<PlateComponent>();
	plateObj3.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider3 = plateObj3.AddComponent<RectColliderComponent>();
	plateCollider3.Init({ 0, 8, 104, 4 }, 100, true, { 0, 133, 133, 255 });
	plateObj3.SetLocalPosition(336.f, 800.f);
	auto& plateSubje3 = plateCollider3.GetSubject();
	plateSubje3.AddObserver(plateCmpt3);

	auto& plateObj4 = sceneLevel1.CreateObject("plateObj4");
	auto& plateCmpt4 = plateObj4.AddComponent<PlateComponent>();
	plateObj4.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider4 = plateObj4.AddComponent<RectColliderComponent>();
	plateCollider4.Init({ 0, 8, 104, 4 }, 100, true, { 0, 133, 133, 255 });
	plateObj4.SetLocalPosition(470.f, 800.f);
	auto& plateSubje4 = plateCollider4.GetSubject();
	plateSubje4.AddObserver(plateCmpt4);
#pragma endregion

#pragma region UI/SCORE
	int uiFontSize{ 20 };
	auto uiFont = ResourceManager::GetInstance().LoadFont("Fonts/BurgerTime_Font.otf", uiFontSize);

	//
	//Static UI
	//
	auto& oneUpTextObject = sceneLevel1.CreateObject("oneUpTextObject");
	oneUpTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 255, 0, 0 }, "1UP");
	oneUpTextObject.SetLocalPosition(float(uiFontSize * 4), 5);

	auto& hiScoreTextObject = sceneLevel1.CreateObject("hiScoreTextObject");
	hiScoreTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 255, 0, 0 }, "HI-SCORE");
	hiScoreTextObject.SetLocalPosition(float(uiFontSize * 4) + (uiFontSize * 5), 5);

	auto& pepperTextObject = sceneLevel1.CreateObject("pepperTextObject");
	pepperTextObject.AddComponent<TextComponent>().Setup(uiFont, SDL_Color{ 0, 255, 0 }, "PEPPER");
	pepperTextObject.SetLocalPosition(float(windowW - (uiFontSize * 7)), 5);

	//
	//Dynamic UI
	//
	auto& peterLivesObj = sceneLevel1.CreateObject("peterLivesObj");
	auto& livescmpt = peterLivesObj.AddComponent<LivesComponent>();
	livescmpt.SetTexture("UI/Peter_Life_Icon.png", 3.25f, 3.25f);
	peterLivesObj.SetLocalPosition(10.f, windowH - 40.f);
	auto& petterPeppersubje = petercmpt.GetSubject();
	petterPeppersubje.AddObserver(livescmpt);



	auto& scoreObject = sceneLevel1.CreateObject("scoreObject");
	auto& scoretextcomp = scoreObject.AddComponent<TextComponent>();
	scoretextcomp.Setup(uiFont);
	scoreObject.AddComponent<ScoreComponent>().SetTextComponent(scoretextcomp);
	scoreObject.SetLocalPosition(uiFontSize * 7.f, uiFontSize * 1.5f);

	//TODO: hi score component should read hi-score from file and observe the score object,
	//if the score components value is bigger than the current high score update it

	auto& hiScoreObject = sceneLevel1.CreateObject("scoreObject");
	auto& hiScoretextcomp = hiScoreObject.AddComponent<TextComponent>();
	hiScoretextcomp.Setup(uiFont);
	hiScoreObject.AddComponent<HiScoreComponent>().SetTextComponent(hiScoretextcomp);
	hiScoreObject.SetLocalPosition(uiFontSize * 15.f, uiFontSize * 1.5f);

	auto& pepperCountObject = sceneLevel1.CreateObject("pepperCountObject");
	auto& pepperCountComp = pepperCountObject.AddComponent<TextComponent>();
	pepperCountComp.Setup(uiFont);
	pepperCountObject.AddComponent<PepperCountComponent>().SetTextComponent(pepperCountComp);
	pepperCountObject.SetLocalPosition(windowW - (uiFontSize * 2.f), uiFontSize * 1.5f);
#pragma endregion


	/*sound system testing code
	//ServiceLocator::RegisterSoundSystem(new LoggingSoundSystem(new SDLSoundSystem("../Data/Sound/")));
	//ServiceLocator::RegisterSoundSystem(new SDLSoundSystem("../Data/Sound/"));
	//ServiceLocator::GetSS().RegisterSound("test.wav", 0);
	//ServiceLocator::GetSS().RegisterSound("test2.wav", 1);
	//ServiceLocator::GetSS().RegisterSound("test3.wav", 2);
	//ServiceLocator::GetSS().PlaySFX(0, 40);
	//ServiceLocator::GetSS().PlaySFX(1, 50);
	//ServiceLocator::GetSS().PlaySFX(2, 80);
	*/


	/*
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
		subje2.AddObserver(scorecomp2);*/




#pragma endregion

	engine->Run();

	engine->Cleanup();
	delete engine;

	return 0;
}