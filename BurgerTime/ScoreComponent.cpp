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

int ScoreComponent::m_CurrentScore{};

ScoreComponent::ScoreComponent(GameObject* pParentObject)
	: m_Subject{}
	, m_pParentObject{ nullptr }
	, m_pTextComponent{ nullptr }

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

void ScoreComponent::Notify(GameObject*, int event)
{
	//harcoded points switch for each event that should give the player points
	switch (Events(event))
	{
	case dae::Events::Item_Fell:
		m_CurrentScore += 50;
		break;
	case dae::Events::Mr_HotDog_Died:
		m_CurrentScore += 100;
		break;
	case dae::Events::Mr_Egg_Died:
		m_CurrentScore += 300;
		break;
	case dae::Events::Mr_Pickle_Died:
		m_CurrentScore += 200;
		break;
	case dae::Events::Drop_Enemy_1:
		m_CurrentScore += 500;
		break;
	case dae::Events::Drop_Enemy_2:
		m_CurrentScore += 1000;
		break;
	case dae::Events::Drop_Enemy_3:
		m_CurrentScore += 2000;
		break;
	case dae::Events::Drop_Enemy_4:
		m_CurrentScore += 4000;
		break;
	case dae::Events::Drop_Enemy_5:
		m_CurrentScore += 8000;
		break;
	case dae::Events::Drop_Enemy_6:
		m_CurrentScore += 16'000;
		break;
	}

	//if the player reaches a certain point ammount it should give the player an extra life
	if (m_CurrentScore >= m_ExtraLifeTreshold)
	{
		m_Subject.Notify(m_pParentObject, static_cast<int>(Events::Extra_Life_Gained));
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
