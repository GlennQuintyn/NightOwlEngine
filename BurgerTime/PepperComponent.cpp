#include "BurgerTimePCH.h"
#include "PepperComponent.h"

dae::PepperComponent::PepperComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_PepperedDuration{ 5.f }
{
}

void dae::PepperComponent::Notify(GameObject* pObject, int event)
{
	pObject;
	event;
}
