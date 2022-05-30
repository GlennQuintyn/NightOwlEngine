#include "BurgerTimePCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

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
#include "SpriteManagerComponent.h"

#include "HiScoreComponent.h"
#include "LivesComponent.h"
#include "PepperCountComponent.h"

#include "RectColliderComponent.h"
#include "LadderComponent.h"
#include "WalkingPlatformComponent.h"
#include "MovementComponent.h"

#include "IngredientComponent.h"
#include "PlateComponent.h"
#pragma endregion

#include "BurgerTimeCommands.h"

#include "ServiceLocator.h"
#include "SDLSoundSystem.h"

using namespace dae;

int main(int, char* [])
{
	auto engine = new dae::NightOwlEngine(640, 900);

	engine->Initialize();

	// tell the resource manager where it can find the game data
	ResourceManager::GetInstance().Init("../Data/");

#pragma region LoadGame

	//std::cout << "\n\n-player 1:\n\tButton Y: Lose a life\n\tButton R1: add 25 to score\n\tButton R3: add 50 to score\n\n";
	//std::cout << "-player 2:\n\tButton X: Lose a life\n\tButton L1: add 25 to score\n\tButton L3: add 50 to score\n\n";

	auto& scene = SceneManager::GetInstance().CreateScene("BURGER TIME");
	auto& inputmanager = InputManager::GetInstance();

	auto fpsFont = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 19);
	//auto normalfont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	int windowW{}, windowH{};
	SDL_GetWindowSize(engine->GetWindow(), &windowW, &windowH);

	auto& backgroundObject = scene.CreateObject("backgroundObject");
	//backgroundObject.AddComponent<Texture2DComponent>().SetTexture("background_adj.jpg");
	backgroundObject.AddComponent<Texture2DComponent>().SetTexture("Level/Level1.png");


	auto& fpsCounterObj = scene.CreateObject("fpsCounterObj");
	auto& fpstextComponent = fpsCounterObj.AddComponent<TextComponent>();
	auto& fpsComponent = fpsCounterObj.AddComponent<FPSComponent>();
	fpsComponent.SetTextComponent(fpstextComponent);
	fpsCounterObj.SetLocalPosition(5.f, 5.f);
	fpstextComponent.SetFont(fpsFont);
	fpstextComponent.SetTextColor({ 0, 255, 255 });

	//peter pepper test object
	auto& peterPepperObj = scene.CreateObject("peterPepperObj");
	auto& petercmpt = peterPepperObj.AddComponent<PeterPepper>();
	auto& peterSpriteManager = peterPepperObj.AddComponent<SpriteManagerComponent>();
	peterSpriteManager.AddSprite("peter/Peter_Idle.png", 1, 1, 0, 45, 45);
	peterSpriteManager.AddSprite("peter/Peter_Walking_Left.png", 4, 1, 16, 45, 45);
	peterSpriteManager.AddSprite("peter/Peter_Walking_Right.png", 4, 1, 16, 45, 45);
	peterSpriteManager.AddSprite("peter/Peter_Walking_Up.png", 4, 1, 16, 45, 45);
	peterSpriteManager.AddSprite("peter/Peter_Walking_Down.png", 4, 1, 16, 45, 45);

	auto& pepperCollider = peterPepperObj.AddComponent<RectColliderComponent>();
	pepperCollider.Init({ 0,0,45,45 }, -1, true);
	peterPepperObj.AddComponent<MovementComponent>();
	peterPepperObj.SetLocalPosition(298, 563);


	//auto& mrHotDogObj = scene.CreateObject("mrHotDogObj");
	//auto& petercmpt = mrHotDogObj.AddComponent<hot>();
	//auto& peterSpriteManager = mrHotDogObj.AddComponent<SpriteManagerComponent>();
	//peterSpriteManager.AddSprite("peter/Peter_Idle.png", 1, 1, 0, 45, 45);
	//peterSpriteManager.AddSprite("peter/Peter_Walking_Left.png", 4, 1, 16, 45, 45);
	//peterSpriteManager.AddSprite("peter/Peter_Walking_Right.png", 4, 1, 16, 45, 45);
	//peterSpriteManager.AddSprite("peter/Peter_Walking_Up.png", 4, 1, 16, 45, 45);
	//peterSpriteManager.AddSprite("peter/Peter_Walking_Down.png", 4, 1, 16, 45, 45);
	//
	//auto& pepperCollider = peterPepperObj.AddComponent<RectColliderComponent>();
	//pepperCollider.Init({ 0,0,45,45 }, -1, true);
	//peterPepperObj.AddComponent<MovementComponent>();

	//seting up the collider logic tree
	//auto& peterColliderlogicObj = peterPepperObj.CreateAddChild("peterColliderLogic");
	//auto& colliderLObj = peterColliderlogicObj.CreateAddChild("Lcollider");
	//auto& colliderLcmpt = colliderLObj.AddComponent<RectColliderComponent>();
	//colliderLcmpt.Init({ -5,37,3,3 }, 0, true, { 255, 0, 0, 128 });
	//auto& colliderRObj = peterColliderlogicObj.CreateAddChild("Rcollider");
	//auto& colliderRcmpt = colliderRObj.AddComponent<RectColliderComponent>();
	//colliderRcmpt.Init({ 47,37,3,3 }, 1, true, { 0, 255, 0, 128 });
	//auto& colliderUPObj = peterColliderlogicObj.CreateAddChild("UPcollider");
	//auto& colliderUPcmpt = colliderUPObj.AddComponent<RectColliderComponent>();
	//colliderUPcmpt.Init({ 22,-5,3,3 }, 2, true, { 0, 0, 255, 128 });
	//auto& colliderDOWNObj = peterColliderlogicObj.CreateAddChild("DOWNcollider");
	//auto& colliderDowncmpt = colliderDOWNObj.AddComponent<RectColliderComponent>();
	//colliderDowncmpt.Init({ 22,50,3,3 }, 3, true, { 0, 255, 255, 128 });

#pragma region InputCommandsPeter
	inputmanager.AddCommand<WalkRightCommand>(PCController::ControllerButton::Button_DPAD_RIGHT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkRightCommand>(InputManager::KeyboardKey::Key_D, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);

	inputmanager.AddCommand<WalkLeftCommand>(PCController::ControllerButton::Button_DPAD_LEFT, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkLeftCommand>(InputManager::KeyboardKey::Key_A, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);

	inputmanager.AddCommand<WalkUpCommand>(PCController::ControllerButton::Button_DPAD_UP, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkUpCommand>(InputManager::KeyboardKey::Key_W, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);

	inputmanager.AddCommand<WalkDownCommand>(PCController::ControllerButton::Button_DPAD_DOWN, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
	inputmanager.AddCommand<WalkDownCommand>(InputManager::KeyboardKey::Key_S, InputManager::ButtonPressState::PressedContinuous).SetPlayer(&peterPepperObj);
#pragma endregion

#pragma region LadderSetup
	auto& ladderObj1 = scene.CreateObject("ladderObj1");
	ladderObj1.SetLocalPosition(51.f, 342.f);
	ladderObj1.AddComponent<LadderComponent>();
	auto& ladder1colliderCmpt = ladderObj1.AddComponent<RectColliderComponent>();
	ladder1colliderCmpt.Init({ 0, 0, 3, 268 }, 10, true, { 255, 255, 0, 128 });

	auto& ladderObj2 = scene.CreateObject("ladderObj2");
	ladderObj2.SetLocalPosition(51.f, 159.f);
	ladderObj2.AddComponent<LadderComponent>();
	auto& ladder2colliderCmpt = ladderObj2.AddComponent<RectColliderComponent>();
	ladder2colliderCmpt.Init({ 0, 0, 3, 137 }, 11, true, { 255, 255, 0, 128 });

	auto& ladderObj3 = scene.CreateObject("ladderObj2");
	ladderObj3.SetLocalPosition(117.f, 251.f);
	ladderObj3.AddComponent<LadderComponent>();
	auto& ladder3colliderCmpt = ladderObj3.AddComponent<RectColliderComponent>();
	ladder3colliderCmpt.Init({ 0, 0, 4, 268 }, 12, true, { 255, 255, 0, 128 });

	auto& ladderObj4 = scene.CreateObject("ladderObj4");
	ladderObj4.SetLocalPosition(184.f, 159.f);
	ladderObj4.AddComponent<LadderComponent>();
	auto& ladder4colliderCmpt = ladderObj4.AddComponent<RectColliderComponent>();
	ladder4colliderCmpt.Init({ 0, 0, 4, 450 }, 13, true, { 255, 255, 0, 128 });

	auto& ladderObj5 = scene.CreateObject("ladderObj5");
	ladderObj5.SetLocalPosition(251.f, 159.f);
	ladderObj5.AddComponent<LadderComponent>();
	auto& ladder5colliderCmpt = ladderObj5.AddComponent<RectColliderComponent>();
	ladder5colliderCmpt.Init({ 0, 0, 4, 182 }, 14, true, { 255, 255, 0, 128 });

	auto& ladderObj6 = scene.CreateObject("ladderObj6");
	ladderObj6.SetLocalPosition(318.f, 159.f);
	ladderObj6.AddComponent<LadderComponent>();
	auto& ladder6colliderCmpt = ladderObj6.AddComponent<RectColliderComponent>();
	ladder6colliderCmpt.Init({ 0, 0, 4, 450 }, 15, true, { 255, 255, 0, 128 });

	auto& ladderObj7 = scene.CreateObject("ladderObj7");
	ladderObj7.SetLocalPosition(385.f, 251.f);
	ladderObj7.AddComponent<LadderComponent>();
	auto& ladder7colliderCmpt = ladderObj7.AddComponent<RectColliderComponent>();
	ladder7colliderCmpt.Init({ 0, 0, 4, 178 }, 16, true, { 255, 255, 0, 128 });

	auto& ladderObj8 = scene.CreateObject("ladderObj8");
	ladderObj8.SetLocalPosition(452.f, 159.f);
	ladderObj8.AddComponent<LadderComponent>();
	auto& ladder8colliderCmpt = ladderObj8.AddComponent<RectColliderComponent>();
	ladder8colliderCmpt.Init({ 0, 0, 4, 450 }, 17, true, { 255, 255, 0, 128 });

	auto& ladderObj9 = scene.CreateObject("ladderObj9");
	ladderObj9.SetLocalPosition(519.f, 338.f);
	ladderObj9.AddComponent<LadderComponent>();
	auto& ladder9colliderCmpt = ladderObj9.AddComponent<RectColliderComponent>();
	ladder9colliderCmpt.Init({ 0, 0, 3, 269 }, 18, true, { 255, 255, 0, 128 });

	auto& ladderObj10 = scene.CreateObject("ladderObj10");
	ladderObj10.SetLocalPosition(586.f, 159.f);
	ladderObj10.AddComponent<LadderComponent>();
	auto& ladder10colliderCmpt = ladderObj10.AddComponent<RectColliderComponent>();
	ladder10colliderCmpt.Init({ 0, 0, 4, 227 }, 19, true, { 255, 255, 0, 128 });

	auto& ladderObj11 = scene.CreateObject("ladderObj11");
	ladderObj11.SetLocalPosition(586.f, 429.f);
	ladderObj11.AddComponent<LadderComponent>();
	auto& ladder11colliderCmpt = ladderObj11.AddComponent<RectColliderComponent>();
	ladder11colliderCmpt.Init({ 0, 0, 4, 180 }, 20, true, { 255, 255, 0, 128 });
#pragma endregion

#pragma region WalkingPlatformSetup
	auto& walkPlatformObj1 = scene.CreateObject("walkPlatformObj1");
	walkPlatformObj1.SetLocalPosition(30.f, 202.f);
	walkPlatformObj1.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform1colliderCmpt = walkPlatformObj1.AddComponent<RectColliderComponent>();
	walkPlatform1colliderCmpt.Init({ 0, 0, 580, 4 }, 50, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj2 = scene.CreateObject("walkPlatformObj2");
	walkPlatformObj2.SetLocalPosition(30.f, 292.f);
	walkPlatformObj2.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform2colliderCmpt = walkPlatformObj2.AddComponent<RectColliderComponent>();
	walkPlatform2colliderCmpt.Init({ 0, 0, 178, 4 }, 51, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj3 = scene.CreateObject("walkPlatformObj3");
	walkPlatformObj3.SetLocalPosition(295.f, 292.f);
	walkPlatformObj3.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform3colliderCmpt = walkPlatformObj3.AddComponent<RectColliderComponent>();
	walkPlatform3colliderCmpt.Init({ 0, 0, 310, 4 }, 52, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj4 = scene.CreateObject("walkPlatformObj4");
	walkPlatformObj4.SetLocalPosition(165.f, 337.f);
	walkPlatformObj4.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform4colliderCmpt = walkPlatformObj4.AddComponent<RectColliderComponent>();
	walkPlatform4colliderCmpt.Init({ 0, 0, 180, 4 }, 53, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj5 = scene.CreateObject("walkPlatformObj5");
	walkPlatformObj5.SetLocalPosition(30.f, 382.f);
	walkPlatformObj5.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform5colliderCmpt = walkPlatformObj5.AddComponent<RectColliderComponent>();
	walkPlatform5colliderCmpt.Init({ 0, 0, 178, 4 }, 54, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj6 = scene.CreateObject("walkPlatformObj6");
	walkPlatformObj6.SetLocalPosition(430.f, 382.f);
	walkPlatformObj6.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform6colliderCmpt = walkPlatformObj6.AddComponent<RectColliderComponent>();
	walkPlatform6colliderCmpt.Init({ 0, 0, 178, 4 }, 55, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj7 = scene.CreateObject("walkPlatformObj7");
	walkPlatformObj7.SetLocalPosition(165.f, 425.f);
	walkPlatformObj7.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform7colliderCmpt = walkPlatformObj7.AddComponent<RectColliderComponent>();
	walkPlatform7colliderCmpt.Init({ 0, 0, 310, 4 }, 56, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj8 = scene.CreateObject("walkPlatformObj8");
	walkPlatformObj8.SetLocalPosition(430.f, 470.f);
	walkPlatformObj8.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform8colliderCmpt = walkPlatformObj8.AddComponent<RectColliderComponent>();
	walkPlatform8colliderCmpt.Init({ 0, 0, 178, 4 }, 57, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj9 = scene.CreateObject("walkPlatformObj9");
	walkPlatformObj9.SetLocalPosition(30.f, 515.f);
	walkPlatformObj9.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform9colliderCmpt = walkPlatformObj9.AddComponent<RectColliderComponent>();
	walkPlatform9colliderCmpt.Init({ 0, 0, 445, 4 }, 58, true, { 0, 255, 0, 128 });

	auto& walkPlatformObj10 = scene.CreateObject("walkPlatformObj10");
	walkPlatformObj10.SetLocalPosition(30.f, 604.f);
	walkPlatformObj10.AddComponent<WalkingPlatformComponent>();
	auto& walkPlatform10colliderCmpt = walkPlatformObj10.AddComponent<RectColliderComponent>();
	walkPlatform10colliderCmpt.Init({ 0, 0, 580, 4 }, 59, true, { 0, 255, 0, 128 });
#pragma endregion


	auto& subje1 = pepperCollider.GetSubject();
	subje1.AddObserver(petercmpt);

	//auto& subjectL = colliderLcmpt.GetSubject();
	//subjectL.AddObserver(petercmpt);
	//auto& subjectR = colliderRcmpt.GetSubject();
	//subjectR.AddObserver(petercmpt);
	//auto& subjectUp = colliderUPcmpt.GetSubject();
	//subjectUp.AddObserver(petercmpt);
	//auto& subjectDown = colliderDowncmpt.GetSubject();
	//subjectDown.AddObserver(petercmpt);
	//auto& subje1 = petercmpt.GetSubject();
	////subje1.AddObserver(pepperCollider);
	//subje1.AddObserver(ladder1colliderCmpt);
	//subje1.AddObserver(pepperCollider);
	//subje3.AddObserver(ladder1colliderCmpt);
	//auto& subje2 = ladder1colliderCmpt.GetSubject();
	//subje2.AddObserver(petercmpt);
	//subje2.AddObserver(pepperCollider);

#pragma region IngredientsSetup
	auto& bunBottomObj1 = scene.CreateObject("bunBottomObj1");
	auto& bunBottomcmpt = bunBottomObj1.AddComponent<IngredientComponent>();
	bunBottomObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Bottom.png", 88.f, 22.f);
	auto& bunBottomcollider1 = bunBottomObj1.AddComponent<RectColliderComponent>();
	bunBottomcollider1.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunBottomObj1.SetLocalPosition(75.f, 595.f);

	auto& burgerMeatObj1 = scene.CreateObject("burgerMeatObj1");
	auto& burgerBottomcmpt = burgerMeatObj1.AddComponent<IngredientComponent>();
	burgerMeatObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Meat.png", 88.f, 22.f);
	auto& burgerMeatcollider1 = burgerMeatObj1.AddComponent<RectColliderComponent>();
	burgerMeatcollider1.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	burgerMeatObj1.SetLocalPosition(75.f, 505.f);

	auto& lettuceObj1 = scene.CreateObject("lettuceObj1");
	auto& lettucecmpt = lettuceObj1.AddComponent<IngredientComponent>();
	lettuceObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Lettuce.png", 88.f, 22.f);
	auto& lettucecollider1 = lettuceObj1.AddComponent<RectColliderComponent>();
	lettucecollider1.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	lettuceObj1.SetLocalPosition(75.f, 375.f);

	auto& bunTopObj1 = scene.CreateObject("bunTopObj1");
	auto& bunTopcmpt = bunTopObj1.AddComponent<IngredientComponent>();
	bunTopObj1.AddComponent<Texture2DComponent>().SetTexture("Burger/Burger_Bun_Top.png", 88.f, 22.f);
	auto& bunTopcollider1 = bunTopObj1.AddComponent<RectColliderComponent>();
	bunTopcollider1.Init({ 0, 0, 88, 20 }, 100, true, { 133, 0, 133, 255 });
	bunTopObj1.SetLocalPosition(75.f, 285.f);
#pragma endregion

#pragma region PlateSetup
	auto& plateObj1 = scene.CreateObject("plateObj1");
	auto& plateCmpt1 = plateObj1.AddComponent<PlateComponent>();
	plateObj1.AddComponent<Texture2DComponent>().SetTexture("Level/Burger_Holder.png", 104.f, 14.f);
	auto& plateCollider1 = plateObj1.AddComponent<RectColliderComponent>();
	plateCollider1.Init({ 0, 8, 104, 4 }, 100, true, { 0, 133, 133, 255 });
	plateObj1.SetLocalPosition(67.f, 800.f);
	//plateObj1.SetLocalPosition(67.f, 690.f);
#pragma endregion

	auto& yeso = plateCollider1.GetSubject();
	yeso.AddObserver(plateCmpt1);
	auto& yeso1 = bunBottomcollider1.GetSubject();
	yeso1.AddObserver(bunBottomcmpt);
	auto& yeso2 = burgerMeatcollider1.GetSubject();
	yeso2.AddObserver(burgerBottomcmpt);
	auto& yeso3 = lettucecollider1.GetSubject();
	yeso3.AddObserver(lettucecmpt);
	auto& yeso4 = bunTopcollider1.GetSubject();
	yeso4.AddObserver(bunTopcmpt);

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