#include "BurgerTimePCH.h"
#include "MrHotDog.h"
#include <GameObject.h>
#include <SpriteManagerComponent.h>
#include "IngredientComponent.h"
#include "PepperComponent.h"
//#include "Enums.h"
//#include "EnemyControllerComponent.h"

dae::MrHotDog::MrHotDog(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_pIngredientWalkingOn{ nullptr }
	, m_pEnemyController{ nullptr }
	, m_RespawnPos{}
	, m_State{}
	, m_SpawnWalkDirection{}
	, m_DeathDurationLength{}
	, m_RespawnDelay{ 1.f }
	, m_DurationLeft{ 0.f }
	, m_ResetInNextUpdate{ false }
{
}

void dae::MrHotDog::SetRespawnPosAndWalkDirection(float x, float y, EnemyControllerComponent::MovementState direction)
{
	m_RespawnPos = { x, y };
	m_SpawnWalkDirection = direction;
}

void dae::MrHotDog::LateInit()
{
	m_pEnemyController = m_pParentObject->GetComponent<EnemyControllerComponent>();
	if (!m_pEnemyController)
	{
		Logger::GetInstance().LogError("MrHotDog: No EnemyControllerComponent was found!");
	}

	//enable movemetn at the start
	//if (m_pEnemyController)
	//{
	//	m_pEnemyController->SetMovement(true);
	//}

	Reset();
}

void dae::MrHotDog::Update()
{
	if (m_ResetInNextUpdate)
	{
		Reset();
		m_ResetInNextUpdate = false;
	}

	//if he walks on a falling ingredient he should die as it falls from out under him
	if (m_pIngredientWalkingOn && m_pIngredientWalkingOn->IsFalling())
	{
		m_State = EnemyState::Dead;
		m_DurationLeft = m_DeathDurationLength;
		m_pEnemyController->SetMovement(false);

		auto pSpriteManager = m_pParentObject->GetComponent<SpriteManagerComponent>();
		if (pSpriteManager)
		{
			pSpriteManager->PlaySprite(static_cast<uint32_t>(SpriteIndices::Death), SpriteManagerComponent::SpritePlayType::PlayOnce);
		}
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

void dae::MrHotDog::Reset()
{
	m_pParentObject->SetLocalPosition(m_RespawnPos);
}

void dae::MrHotDog::PlaceOffScreen()
{
	//placing ofscreen lets the colliderboxes reset
	m_ResetInNextUpdate = true;
	m_pParentObject->SetLocalPosition(-1000.f, -1000.f);
}

void dae::MrHotDog::Notify(GameObject* pObject, int event)
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
		//m_ResetInNextUpdate = true;
		//Reset();
		PlaceOffScreen();
		m_State = EnemyState::WaitingToMove;
		m_DurationLeft = m_RespawnDelay;
	}
	else if (event == static_cast<int>(Events::Game_Won))
	{
		//m_State = EnemyState::WaitingToMove;
		m_pEnemyController->SetMovement(false);
		//Reset();
	}
}
