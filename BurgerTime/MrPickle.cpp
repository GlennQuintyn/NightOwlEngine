#include "BurgerTimePCH.h"
#include "MrPickle.h"
#include <GameObject.h>
#include <SpriteManagerComponent.h>
#include "IngredientComponent.h"
#include "PepperComponent.h"
#include "ServiceLocator.h"

dae::MrPickle::MrPickle(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_pEnemyController{ nullptr }
	, m_pIngredientWalkingOn{}
	, m_RespawnPos{}
	, m_State{}
	, m_SpawnWalkDirection{}
	, m_DeathDurationLength{}
	, m_RespawnDelay{ 1.f }
	, m_DurationLeft{ 0.f }
	, m_ResetInNextUpdate{ false }
{
}
void dae::MrPickle::SetRespawnPosAndWalkDirection(float x, float y, EnemyControllerComponent::MovementState direction)
{
	m_RespawnPos = { x, y };
	m_SpawnWalkDirection = direction;
}

void dae::MrPickle::LateInit()
{
	m_pEnemyController = m_pParentObject->GetComponent<EnemyControllerComponent>();
	if (!m_pEnemyController)
	{
		Logger::GetInstance().LogError("MrPickle: No EnemyControllerComponent was found!");
	}

	Reset();
}

void dae::MrPickle::Update()
{
	if (m_ResetInNextUpdate)
	{
		Reset();
		m_ResetInNextUpdate = false;
	}

	//if he walks on a falling ingredient he should die as it falls from out under him
	if (m_State != EnemyState::Dead && m_pIngredientWalkingOn && m_pIngredientWalkingOn->IsFalling())
	{
		m_State = EnemyState::Dead;
		m_DurationLeft = m_DeathDurationLength;

		m_pEnemyController->SetMovement(false);

		auto pSpriteManager = m_pParentObject->GetComponent<SpriteManagerComponent>();
		if (pSpriteManager)
		{
			pSpriteManager->PlaySprite(static_cast<uint32_t>(SpriteIndices::Death), SpriteManagerComponent::SpritePlayType::PlayOnce);
		}

		ServiceLocator::GetSS().PlaySFX(static_cast<int>(SoundIndices::EnemyCrushed), 40);
	}

	switch (m_State)
	{
	case EnemyState::Dead:
		m_DurationLeft -= Time::GetInstance().GetDeltaT();
		if (m_DurationLeft <= 0.f)
		{
			PlaceOffScreen();
			//Reset();
			m_State = EnemyState::WaitingToMove;
			m_DurationLeft = m_RespawnDelay;

			auto pSpriteManager = m_pParentObject->GetComponent<SpriteManagerComponent>();
			if (pSpriteManager)
			{
				pSpriteManager->PlaySprite(static_cast<uint32_t>(SpriteIndices::Idle), SpriteManagerComponent::SpritePlayType::LoopUntilNoMovement);
			}
		}
		break;
	case EnemyState::Peppered:
		m_DurationLeft -= Time::GetInstance().GetDeltaT();
		if (m_DurationLeft <= 0.f)
		{
			m_State = EnemyState::Moving;

			m_pEnemyController->SetMovement(true);
		}
		break;
	case EnemyState::WaitingToMove:
		m_DurationLeft -= Time::GetInstance().GetDeltaT();
		if (m_DurationLeft <= 0.f)
		{
			m_pEnemyController->SetMovement(true, m_SpawnWalkDirection);

			m_State = EnemyState::Moving;
		}
		break;
	}
}

void dae::MrPickle::Reset()
{
	m_pParentObject->SetLocalPosition(m_RespawnPos);
}

void dae::MrPickle::PlaceOffScreen()
{
	//placing ofscreen lets the colliderboxes reset
	m_ResetInNextUpdate = true;
	m_pParentObject->SetLocalPosition(-1000.f, -1000.f);
}

void dae::MrPickle::Notify(GameObject* pObject, int event)
{
	//if it entered a ingredient collider while it was falling, then it should DIE and play its death anim (not looping)
	if (event == 0)
	{
		auto pIngredient = pObject->GetComponent<IngredientComponent>();

		if (pIngredient)
		{
			m_pIngredientWalkingOn = pIngredient;

			if (pIngredient->IsFalling())
			{
				m_State = EnemyState::Dead;
				m_DurationLeft = m_DeathDurationLength;

				m_pEnemyController->SetMovement(false);

				auto pSpriteManager = m_pParentObject->GetComponent<SpriteManagerComponent>();
				if (pSpriteManager)
				{
					pSpriteManager->PlaySprite(static_cast<uint32_t>(SpriteIndices::Death), SpriteManagerComponent::SpritePlayType::PlayOnce);
				}

				ServiceLocator::GetSS().PlaySFX(static_cast<int>(SoundIndices::EnemyCrushed), 40);
			}
		}

		if (m_State == EnemyState::Dead)//if enemy is dead they can't be peppered anymore
			return;

		auto pPepper = pObject->GetComponent<PepperComponent>();

		if (pPepper)
		{
			m_DurationLeft = pPepper->GetPepperedDuration();

			m_pEnemyController->SetMovement(false);

			m_State = EnemyState::Peppered;

			auto pSpriteManager = m_pParentObject->GetComponent<SpriteManagerComponent>();
			if (pSpriteManager)
			{
				pSpriteManager->PlaySprite(static_cast<uint32_t>(SpriteIndices::Peppered), SpriteManagerComponent::SpritePlayType::Looping);
			}

			ServiceLocator::GetSS().PlaySFX(static_cast<int>(SoundIndices::PepperThrown), 40);
		}
	}
	else if (event == 1)
	{
		auto pIngredient = pObject->GetComponent<IngredientComponent>();

		if (pIngredient)
		{
			m_pIngredientWalkingOn = nullptr;
		}
	}
	else if (event == static_cast<int>(Events::Player_Died))
	{
		m_pEnemyController->SetMovement(false);
	}
	else if (event == static_cast<int>(Events::Reset_Pos))
	{
		PlaceOffScreen();
		m_State = EnemyState::WaitingToMove;
		m_DurationLeft = m_RespawnDelay;
	}
	else if (event == static_cast<int>(Events::Game_Won))
	{
		m_pEnemyController->SetMovement(false);
	}
}