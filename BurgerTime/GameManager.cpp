#include "BurgerTimePCH.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "Enums.h"

dae::GameManager::GameManager(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_NextAction{ NextAction::Nothing }
	, m_Subject{}
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
	}
}

void dae::GameManager::RestartCurrentLevel(bool withDelay)
{
	withDelay;
	if (withDelay)
	{
		m_NextAction = NextAction::RestartCurrentLevel;
		m_TimeLeft = m_ResetDelay;
	}
	else
	{
		m_Subject.Notify(m_pParentObject, static_cast<int>(Events::ResetPos));
	}
}

void dae::GameManager::ReturnToMainMenu(bool withDelay)
{
	if (withDelay)
	{
		m_NextAction = NextAction::ReturnToMainMenu;
		m_TimeLeft = m_ResetDelay;
	}
	else
	{
		//scene 0 should be the main menu scene
		auto& scene = SceneManager::GetInstance();
		scene.SetActiveScene(0);
		scene.LateInit();
	}
}