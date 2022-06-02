#include "BurgerTimePCH.h"

#include <stdio.h>
#include <sstream>
#include "Logger.h"
#include "Renderer.h"
#include "SteamAchievements.h"

#include "ScoreComponent.h"
#include "TextComponent.h"
#include "Enums.h"

using namespace dae;

//extern SteamAchievements* g_SteamAchievements;

ScoreComponent::ScoreComponent(GameObject* pParentObject)
	: m_Subject{}
	, m_pParentObject{ nullptr }
	, m_pTextComponent{ nullptr }
	, m_CurrentScore{}
	, m_ExtraLifeTreshold{ 20'000 }//every 20 000 points the player gets another life
{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("SCORECOMPONENT:\tPARENT OBJECT WAS NOT GIVEN!");
}

void ScoreComponent::SetTextComponent(TextComponent& pTextComponent)
{
	m_pTextComponent = &pTextComponent;
}

void ScoreComponent::Notify(GameObject* pObject, int event)
{
	pObject;

	switch (Events(event))
	{
	case dae::Events::Item_Fell:
		m_CurrentScore += 25;
		break;
	case dae::Events::Enemy_Peppered:
		m_CurrentScore += 50;
		break;
	}

	if (m_CurrentScore >= m_ExtraLifeTreshold)
	{
		m_Subject.Notify(m_pParentObject, static_cast<int>(Events::ExtraLifeGained));
		//if (SteamAchievements::g_SteamAchievements)
		//	SteamAchievements::g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
	}
}

void ScoreComponent::LateInit()
{
	if (!m_pTextComponent)
		Logger::GetInstance().LogError("SCORECOMPONENT:\tTEXTCOMPONENT WAS NOT GIVEN!");
}

void ScoreComponent::Update()
{
	m_pTextComponent->SetText(std::to_string(m_CurrentScore));
}
