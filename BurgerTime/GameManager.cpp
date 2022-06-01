#include "BurgerTimePCH.h"
#include "GameManager.h"
#include "SceneManager.h"

dae::GameManager::GameManager(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_NextAction{ NextAction::Nothing }
	, m_ResetDelay{ 4.5f }
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
			default:
				Logger::GetInstance().LogError("GameManager: invalid switch state reached!");
				break;
			}

			m_NextAction = NextAction::Nothing;
		}
	}
}

void dae::GameManager::Notify(GameObject* pObject, int event)
{
	pObject;
	event;
}

void dae::GameManager::RestartCurrentLevel(bool withDelay)
{
	if (withDelay)
	{

	}
	else
	{
		//SceneManager::GetInstance().
	}
}

void dae::GameManager::ReturnToMainMenu(bool withDelay)
{
	if (withDelay)
	{

	}
}