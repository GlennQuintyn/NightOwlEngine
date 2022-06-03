#include "BurgerTimePCH.h"
#include "HiScoreComponent.h"
#include "TextComponent.h"
#include "ScoreComponent.h"

using namespace dae;

int HiScoreComponent::m_Score{};

HiScoreComponent::HiScoreComponent(GameObject* pParentObject)
	: m_pParentObject{ nullptr }
	, m_pTextComponent{ nullptr }
	, m_pScoreComponent{ nullptr }
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

	//TODO: Add-in score loading from file


	m_pTextComponent->SetText(std::to_string(m_Score));//set intial score value
}

void HiScoreComponent::Update()
{
	if (m_pScoreComponent->GetCurrentScore() > m_Score)
	{
		m_pTextComponent->SetText(std::to_string(m_Score));//update score value
	}
}

void HiScoreComponent::SetTextComponent(TextComponent& textComponent)
{
	m_pTextComponent = &textComponent;
}

void HiScoreComponent::SetScoreComponent(ScoreComponent& scoreComponent)
{
	m_pScoreComponent = &scoreComponent;
}