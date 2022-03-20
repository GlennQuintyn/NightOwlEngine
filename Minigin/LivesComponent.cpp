#include "NightOwlEnginePCH.h"
#include "LivesComponent.h"
#include "Renderer.h"
#include "TextComponent.h"
#include "Enums.h"

using namespace dae;

LivesComponent::LivesComponent(GameObject* pParentObject)
	: m_CurrentLives{ 3 }
	, m_pParentObject{ nullptr }
	, m_pTextComponent{ nullptr }
{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("LIVESCOMPONENT:\tPARENT OBJECT WAS NOT GIVEN!");
}

void LivesComponent::SetTextComponent(TextComponent& pTextComponent)
{
	m_pTextComponent = &pTextComponent;
}

void dae::LivesComponent::Notify(GameObject* pObject, int event)
{
	pObject;

	switch (Events(event))
	{
	case dae::Events::PeterPepper_Died:
		--m_CurrentLives;
		if (m_CurrentLives < 0)
			m_CurrentLives = 0;
		break;
	}

}

void LivesComponent::LateInit()
{
	if (!m_pTextComponent)
		Logger::GetInstance().LogError("LIVESCOMPONENT:\tTEXTCOMPONENT WAS NOT GIVEN!");
}

void dae::LivesComponent::Update()
{
	m_pTextComponent->SetText("Lives: " + std::to_string(m_CurrentLives));
}
