#include "NightOwlEnginePCH.h"
#include "FPSComponent.h"
#include "Renderer.h"
#include "TextComponent.h"

using namespace dae;

FPSComponent::FPSComponent(GameObject* pParentObject)
	: m_pParentObject{ nullptr }
	, m_pTextComponent{ nullptr }
{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("FPSComponent:\tPARENT OBJECT WAS NOT GIVEN!");
}

void FPSComponent::SetTextComponent(TextComponent& pTextComponent)
{
	m_pTextComponent = &pTextComponent;
}

void FPSComponent::LateInit()
{
	if (!m_pTextComponent)
		Logger::GetInstance().LogError("FPSComponent:\tTEXTCOMPONENT WAS NOT GIVEN!");
}

void FPSComponent::Update()
{
	if (m_pTextComponent)
		m_pTextComponent->SetText(std::to_string(int(Time::GetInstance().GetFPS())) + " FPS");
	//m_pTextComponent->SetText(std::format("{:.1f} FPS", Time::GetInstance().GetFPS()));
}
