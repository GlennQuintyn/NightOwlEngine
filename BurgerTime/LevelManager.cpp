#include "BurgerTimePCH.h"
#include "LevelManager.h"
#include <SceneManager.h>
#include "Enums.h"
#include "EnemyControllerComponent.h"
#include <ServiceLocator.h>
#include "ScoreComponent.h"

dae::LevelManager::LevelManager(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_Subject{}
	, m_pEnemyPlayer{}
	, m_pCoopPlayer{}
	, m_pNormalPlayer{}
	, m_pPepperObject{}
	, m_pScoreCmpt{}
	, m_NextAction{ NextAction::Nothing }
	, m_GameMode{ GameMode::MainMenu }
	, m_PlateFullCount{}
	, m_MaxPlateFullCount{}
	, m_LastSceneIndex{}
	, m_Delay{ 4.5f }
	, m_TimeLeft{}
	, m_TimerStarted{}
{
}

void dae::LevelManager::Update()
{
	if (m_NextAction != NextAction::Nothing)
	{
		m_TimeLeft -= Time::GetInstance().GetDeltaT();

		if (m_TimeLeft <= 0.f)
		{
			switch (m_NextAction)
			{
			case LevelManager::NextAction::ResetEnemiesAndPlayer:
				break;
			case LevelManager::NextAction::RestartCurrentLevel:
				RestartCurrentLevel();
				break;
			case LevelManager::NextAction::ReturnToMainMenu:
				ReturnToMainMenu();
				break;
			case LevelManager::NextAction::AdvanceToNextLevel:
				AdvanceToNextLevel();
				break;
			default:
				Logger::GetInstance().LogError("LevelManager: invalid switch state reached!");
				break;
			}

			m_NextAction = NextAction::Nothing;
		}
	}
}

void dae::LevelManager::Reset()
{
	m_NextAction = NextAction::Nothing;
	m_PlateFullCount = 0;

	m_pNormalPlayer->SetEnabledState(true);

	//SET THE COOP PLAYER NULLPTR OR NOT ON THE ENEMIES WHEN PLAYING COOP OR NOT
	switch (m_GameMode)
	{
	case dae::GameMode::SinglePlayer:
	{
		//enable all the normal enemies
		for (auto& pEnemy : m_pEnemies)
		{
			pEnemy->SetEnabledState(true);
			//if in singleplayer then the enemies shouldn't goto the 2nd player
			if (auto pController = pEnemy->GetComponent<EnemyControllerComponent>())
			{
				pController->SetPlayer2(nullptr);
			}
		}

		//disable the enemyplayer
		m_pEnemyPlayer->SetEnabledState(false);
		//disable the coop player
		m_pCoopPlayer->SetEnabledState(false);
		//enable PepperObject of the this level
		m_pPepperObject->SetEnabledState(true);

		break;
	}
	case dae::GameMode::Coop:
	{
		//enable all the normal enemies
		for (auto& pEnemy : m_pEnemies)
		{
			pEnemy->SetEnabledState(true);
			//if in coop, the enemies should be aware of the 2nd player
			if (auto pController = pEnemy->GetComponent<EnemyControllerComponent>())
			{
				pController->SetPlayer2(m_pCoopPlayer);
			}
		}

		//disable the enemyplayer
		m_pEnemyPlayer->SetEnabledState(false);
		//enable the coop player
		m_pCoopPlayer->SetEnabledState(true);
		//enable PepperObject of the this level
		m_pPepperObject->SetEnabledState(true);

		break;
	}
	case dae::GameMode::Versus:
	{
		//disable all the normal enemies
		for (auto& pEnemy : m_pEnemies)
		{
			pEnemy->SetEnabledState(false);
			//if in versus, the enemies shouldn't be aware of the 2nd player and also they aren't there
			if (auto pController = pEnemy->GetComponent<EnemyControllerComponent>())
			{
				pController->SetPlayer2(nullptr);
			}
		}
		//enable the enemyplayer
		m_pEnemyPlayer->SetEnabledState(true);
		//disable the coop player
		m_pCoopPlayer->SetEnabledState(false);
		//enable PepperObject of the this level
		m_pPepperObject->SetEnabledState(true);

		break;
	}
	case dae::GameMode::MainMenu://dont't do anything
		break;
	default:
		Logger::GetInstance().LogError("LevelManager: invalid switch state reached!");
		break;
	}

}

void dae::LevelManager::AddEnemy(GameObject* pEnemy)
{
	m_pEnemies.emplace_back(pEnemy);
}

void dae::LevelManager::Notify(GameObject*, int event)
{
	switch (static_cast<Events>(event))
	{
	case Events::Player_Died:
		RestartCurrentLevel(true);
		break;
	case Events::Game_Over:
		ReturnToMainMenu(true);
		break;
	case Events::Plate_Full:
		++m_PlateFullCount;
		if (m_PlateFullCount >= m_MaxPlateFullCount)
		{
			m_Subject.Notify(m_pParentObject, static_cast<int>(Events::Game_Won));
			AdvanceToNextLevel(true);

			ServiceLocator::GetSS().PlaySFX(static_cast<int>(SoundIndices::LevelWon), 40);
		}
		break;
	}
}

void dae::LevelManager::RestartCurrentLevel(bool withDelay)
{
	withDelay;
	if (withDelay)
	{
		m_NextAction = NextAction::RestartCurrentLevel;
		m_TimeLeft = m_Delay;
	}
	else
	{
		m_Subject.Notify(m_pParentObject, static_cast<int>(Events::Reset_Pos));
	}
}

void dae::LevelManager::ReturnToMainMenu(bool withDelay)
{
	if (withDelay)
	{
		m_NextAction = NextAction::ReturnToMainMenu;
		m_TimeLeft = m_Delay;
	}
	else
	{
		m_pScoreCmpt->ResetScore();

		//scene 0 should be the main menu scene
		auto& scene = SceneManager::GetInstance();
		DisabledAll();
		scene.SetActiveScene(0);
		//re init the mainmenu scene to reset its state
		scene.LateInit();
	}
}

void dae::LevelManager::AdvanceToNextLevel(bool withDelay)
{
	if (withDelay)
	{
		m_NextAction = NextAction::AdvanceToNextLevel;
		m_TimeLeft = m_Delay;
	}
	else
	{
		auto& scene = SceneManager::GetInstance();

		//disable all stuff that changes in mode switch
		DisabledAll();

		if (scene.GetActiveScene() == m_LastSceneIndex)
		{
			//if level 3(last level) was won the player will go back to level 1 and go through all the levels again until he dies, or the end of time comes
			//whatever comes first
			scene.SetActiveScene(1);
		}
		else
		{
			scene.GotoNextScene();
		}

		//scene.LateInit();
		scene.ResetActiveScene();
	}
}

void dae::LevelManager::DisabledAll()
{
	for (auto& pEnemy : m_pEnemies)
	{
		pEnemy->SetEnabledState(false);
	}

	m_pEnemyPlayer->SetEnabledState(false);
	m_pNormalPlayer->SetEnabledState(false);
	m_pCoopPlayer->SetEnabledState(false);

	//disable the pepper object of the noew old scene so that it can't be called upon again
	m_pPepperObject->SetEnabledState(false);
}
