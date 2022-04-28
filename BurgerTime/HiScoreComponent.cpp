#include "BurgerTimePCH.h"
//#include <stdio.h>
//#include <sstream>
//#include "Logger.h"

#include "HiScoreComponent.h"
#include "TextComponent.h"

using namespace dae;

HiScoreComponent::HiScoreComponent(GameObject* pParentObject)
	: m_pParentObject{ nullptr }
	, m_pTextComponent{ nullptr }
	, m_pScoreComponent{ nullptr }
	, m_Score{}
{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("HISCORECOMPONENT:\tPARENT OBJECT WAS NOT GIVEN!");
}

void HiScoreComponent::LateInit()
{
	if (!m_pTextComponent)
		Logger::GetInstance().LogError("HISCORECOMPONENT:\tTEXTCOMPONENT WAS NOT GIVEN!");
	if (!m_pScoreComponent)
		Logger::GetInstance().LogError("HISCORECOMPONENT:\tSCORECOMPONENT WAS NOT GIVEN!");
}

void HiScoreComponent::Update()
{
	m_pTextComponent->SetText(std::to_string(m_Score));
}

void HiScoreComponent::SetTextComponent(TextComponent& textComponent)
{
	m_pTextComponent = &textComponent;
}

void HiScoreComponent::SetScoreComponent(ScoreComponent& scoreComponent)
{
	m_pScoreComponent = &scoreComponent;
}