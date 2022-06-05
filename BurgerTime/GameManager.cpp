#include "BurgerTimePCH.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "Enums.h"

dae::GameManager::GameManager(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_NextAction{ NextAction::Nothing }
	, m_Subject{}
	, m_PlateFullCount{}
	, m_MaxPlateFullCount{}
	, m_LastSceneIndex{}
	, m_Delay{ 4.5f }
	, m_TimeLeft{}
	, m_TimerStarted{}
{
}

void dae::GameManager::Update()
{
	if (m_NextAction != NextAction::Nothing)
	{
		m_TimeLeft -= Time::GetInstance().GetDeltaT();

		if (m_TimeLeft <= 0.f)
		{
			switch (m_NextAction)
			{
			case dae::GameManager::NextAction::RestartCurrentLevel:
				RestartCurrentLevel();
				break;
			case dae::GameManager::NextAction::ReturnToMainMenu:
				ReturnToMainMenu();
				break;
			case dae::GameManager::NextAction::AdvanceToNextLevel:
				AdvanceToNextLevel();
				break;
			default:
				Logger::GetInstance().LogError("GameManager: invalid switch state reached!");
				break;
			}

			m_NextAction = NextAction::Nothing;
		}
	}
}

void dae::GameManager::Reset()
{
	m_NextAction = NextAction::Nothing;
	m_MaxPlateFullCount = 0;
}

void dae::GameManager::Notify(GameObject*, int event)
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
		}
		break;
	}
}

void dae::GameManager::RestartCurrentLevel(bool withDelay)
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

void dae::GameManager::ReturnToMainMenu(bool withDelay)
{
	if (withDelay)
	{
		m_NextAction = NextAction::ReturnToMainMenu;
		m_TimeLeft = m_Delay;
	}
	else
	{
		//scene 0 should be the main menu scene
		auto& scene = SceneManager::GetInstance();
		scene.SetActiveScene(0);
		scene.LateInit();
	}
}

void dae::GameManager::AdvanceToNextLevel(bool withDelay)
{
	if (withDelay)
	{
		m_NextAction = NextAction::AdvanceToNextLevel;
		m_TimeLeft = m_Delay;
	}
	else
	{
		auto& scene = SceneManager::GetInstance();
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
