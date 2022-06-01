#include "BurgerTimePCH.h"
#include "MrHotDog.h"
#include "GameObject.h"
#include "IngredientComponent.h"
#include "SpriteManagerComponent.h"
#include "Enums.h"
#include "PepperComponent.h"
#include "EnemyControllerComponent.h"

dae::MrHotDog::MrHotDog(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_pEnemyController{ nullptr }
	, m_PepperedDurationLeft{ 0.f }
	, m_IsPeppered{ false }
{
}

void dae::MrHotDog::LateInit()
{
	m_pEnemyController = m_pParentObject->GetComponent<EnemyControllerComponent>();
	if (!m_pEnemyController)
	{
		Logger::GetInstance().LogError("MrHotDog: No EnemyControllerComponent was found!");
	}
}

void dae::MrHotDog::Update()
{
	if (m_IsPeppered)
	{
		m_PepperedDurationLeft -= Time::GetInstance().GetDeltaT();
		if (m_PepperedDurationLeft <= 0.f)
		{
			m_IsPeppered = false;
			m_pEnemyController->SetMovement(true);
		}
	}
}

void dae::MrHotDog::Notify(GameObject* pObject, int event)
{
	//if it entered a ingredient collider while it was falling, then it should DIE and play its death anim (not looping)
	if (event == 0)
	{
		auto pIngredient = pObject->GetComponent<IngredientComponent>();

		if (pIngredient && pIngredient->IsFalling())
		{
			auto pSpriteManager = m_pParentObject->GetComponent<SpriteManagerComponent>();
			if (pSpriteManager)
			{
				pSpriteManager->PlaySprite(static_cast<uint32_t>(SpriteIndices::Death), SpriteManagerComponent::SpritePlayType::PlayOnce);
			}
		}

		auto pPepper = pObject->GetComponent<PepperComponent>();

		if (pPepper)
		{
			m_PepperedDurationLeft = pPepper->GetPepperedDuration();

			auto pSpriteManager = m_pParentObject->GetComponent<SpriteManagerComponent>();
			if (pSpriteManager)
			{
				pSpriteManager->PlaySprite(static_cast<uint32_t>(SpriteIndices::Peppered), SpriteManagerComponent::SpritePlayType::Looping);
				m_pEnemyController->SetMovement(false);
				m_IsPeppered = true;
			}
		}
	}
}
