#include "BurgerTimePCH.h"
//#include <stdio.h>
//#include <sstream>
//#include "Logger.h"
#include "PepperCountComponent.h"
#include "TextComponent.h"

using namespace dae;

PepperCountComponent::PepperCountComponent(GameObject* pParentObject)
	: m_pParentObject{ nullptr }
	, m_pTextComponent{ nullptr }
	, m_PepperCount{ 5 }
{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("PEPPERCOUNTCOMPONENT:\tPARENT OBJECT WAS NOT GIVEN!");
}

void PepperCountComponent::SetTextComponent(TextComponent& pTextComponent)
{
	m_pTextComponent = &pTextComponent;
}

void PepperCountComponent::LateInit()
{
	if (!m_pTextComponent)
		Logger::GetInstance().LogError("SCORECOMPONENT:\tTEXTCOMPONENT WAS NOT GIVEN!");
}
void PepperCountComponent::Update()
{
	m_pTextComponent->SetText(std::to_string(m_PepperCount));
}
