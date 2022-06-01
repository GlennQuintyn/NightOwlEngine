#include "BurgerTimePCH.h"
#include "PepperComponent.h"
#include "GameObject.h"

dae::PepperComponent::PepperComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_ResetPos{}
	, m_PepperedDuration{ 5.f }
	, m_SpriteDuration{ 0.f }
	, m_TimeLeft{ 0.f }
	, m_hasBeenThrown{ false }
{
}

void dae::PepperComponent::Update()
{
	if (m_hasBeenThrown)
	{
		m_TimeLeft -= Time::GetInstance().GetDeltaT();

		if (m_TimeLeft <= 0.f)
		{
			Reset();
			m_hasBeenThrown = false;
		}
	}
}

void dae::PepperComponent::Reset()
{
	m_pParentObject->SetLocalPosition(m_ResetPos);
}

void dae::PepperComponent::Thrown()
{
	m_hasBeenThrown = true;
	m_TimeLeft = m_SpriteDuration;
}

void dae::PepperComponent::Notify(GameObject* pObject, int event)
{
	pObject;
	event;
}