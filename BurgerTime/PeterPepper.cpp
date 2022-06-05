#include "BurgerTimePCH.h"
#include "PeterPepper.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Subject.h"
#include "Enums.h"
#include "LadderComponent.h"
#include "RectColliderComponent.h"
#include "WalkingPlatformComponent.h"
#include "SpriteManagerComponent.h"
#include "MovementComponent.h"
#include <ServiceLocator.h>

#include "MrHotDog.h"
#include "MrEgg.h"
#include "MrPickle.h"

using namespace dae;

PeterPepper::PeterPepper(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_Subject{}
	, m_IsAlive{ true }
{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("FPSComponent:\tPARENT OBJECT WAS NOT GIVEN!");
}

void PeterPepper::LateInit()
{
	if (auto pCollidercmpt = m_pParentObject->GetComponent<RectColliderComponent>())
	{
		m_Size.x = pCollidercmpt->GetRectangle().w;
		m_Size.y = pCollidercmpt->GetRectangle().h;
	}

	//enable movemetn at the start
	if (auto pMovement = m_pParentObject->GetComponent<MovementComponent>())
	{
		pMovement->SetEnabled(true);
	}
}

void dae::PeterPepper::Notify(GameObject* pObject, int event)
{
	if (!m_IsAlive)
		return;

	//if it entered a ingredient collider while it was falling, then it should DIE and play its death anim (not looping)
	if (event == 0)
	{
		if (auto pEnemyHotDog = pObject->GetComponent<MrHotDog>())
		{
			if (pEnemyHotDog->IsHostile())
			{
				EnemyTouchedMe();
			}
		}
		else if (auto pEnemyEgg = pObject->GetComponent<MrEgg>())
		{
			if (pEnemyEgg->IsHostile())
			{
				EnemyTouchedMe();
			}
		}
		else if (auto pEnemyPickle = pObject->GetComponent<MrPickle>())
		{
			if (pEnemyPickle->IsHostile())
			{
				EnemyTouchedMe();
			}
		}
	}
	else if (event == static_cast<int>(Events::Reset_Pos))
	{
		Reset();
	}
	else if (event == static_cast<int>(Events::Game_Won))
	{
		if (auto pSpriteManager = m_pParentObject->GetComponent<SpriteManagerComponent>())
		{
			pSpriteManager->PlaySprite(static_cast<uint32_t>(SpriteIndices::Won), SpriteManagerComponent::SpritePlayType::Looping);
		}
		if (auto pMovement = m_pParentObject->GetComponent<MovementComponent>())
		{
			pMovement->SetEnabled(false);
		}
	}
}

//spawn location only gets set at begin of level so movement should be active
void dae::PeterPepper::SetSpawnLocation(float x, float y)
{
	m_SpawnPos.x = x;
	m_SpawnPos.y = y;
}

void dae::PeterPepper::EnemyTouchedMe()
{
	if (auto pSpriteManager = m_pParentObject->GetComponent<SpriteManagerComponent>())
	{
		pSpriteManager->PlaySprite(static_cast<uint32_t>(SpriteIndices::Death), SpriteManagerComponent::SpritePlayType::PlayOnce);
		m_Subject.Notify(m_pParentObject, int(Events::Player_Died));
		if (auto pMovement = m_pParentObject->GetComponent<MovementComponent>())
		{
			pMovement->SetEnabled(false);
		}
	}

	ServiceLocator::GetSS().PlaySFX(static_cast<int>(SoundIndices::PlayerDied), 40);
}

void dae::PeterPepper::Reset()
{
	m_pParentObject->SetLocalPosition(m_SpawnPos);

	if (auto pMovement = m_pParentObject->GetComponent<MovementComponent>())
	{
		pMovement->SetEnabled(true);
	}
	if (auto pSpriteManager = m_pParentObject->GetComponent<SpriteManagerComponent>())
	{
		pSpriteManager->PlaySprite(static_cast<uint32_t>(SpriteIndices::Idle));
	}
}
