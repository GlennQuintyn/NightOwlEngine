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

#include "MrHotDog.h"
#include "MrEgg.h"
#include "MrPickle.h"

using namespace dae;

PeterPepper::PeterPepper(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_Subject{}
	, m_IsAlive{ true }
	//, m_DeahtButton{ PCController::ControllerButton::Button_Triangle }
	//, m_ObjFellButton{ PCController::ControllerButton::Button_R_SHOULDER }
	//, m_EnemyPepperedButton{ PCController::ControllerButton::Button_R_THUMB }
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
}

void PeterPepper::Update()
{
	if (!m_IsAlive)
		return;

	//if (InputManager::GetInstance().IsPressedThisFrame(m_DeahtButton))
	//{
	//	m_Subject.Notify(m_pParentObject, int(Events::PeterPepper_Died));
	//	//m_pParentObject->GetComponent<Subject>()->Notify(m_pParentObject, int(Events::PeterPepper_Died));
	//}
	//if (InputManager::GetInstance().IsPressedThisFrame(m_ObjFellButton))
	//{
	//	m_Subject.Notify(m_pParentObject, int(Events::Item_Fell));
	//	//m_pParentObject->GetComponent<Subject>()->Notify(m_pParentObject, int(Events::Item_Fell));
	//}
	//if (InputManager::GetInstance().IsPressedThisFrame(m_EnemyPepperedButton))
	//{
	//	m_Subject.Notify(m_pParentObject, int(Events::Enemy_Peppered));
	//	//m_pParentObject->GetComponent<Subject>()->Notify(m_pParentObject, int(Events::Enemy_Peppered));
	//}
}

void dae::PeterPepper::Notify(GameObject* pObject, int event)
{
	if (!m_IsAlive)
		return;

	//if it entered a ingredient collider while it was falling, then it should DIE and play its death anim (not looping)
	if (event == 0)
	{
		if (pObject->GetComponent<MrHotDog>() || pObject->GetComponent<MrEgg>() || pObject->GetComponent<MrPickle>())
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
		}
	}
}

//void PeterPepper::SetDeathButton(PCController::ControllerButton deahtButton)
//{
//	m_DeahtButton = deahtButton;
//}
//
//void PeterPepper::SetObjectFellButton(PCController::ControllerButton objFellButton)
//{
//	m_ObjFellButton = objFellButton;
//}
//
//void PeterPepper::SetPepperEnemyButton(PCController::ControllerButton enemyPepperedButton)
//{
//	m_EnemyPepperedButton = enemyPepperedButton;
//}

void dae::PeterPepper::SetSpawnLocation(float x, float y)
{
	m_SpawnLoc.x = x;
	m_SpawnLoc.y = y;

	//TODO: move to RESET FUNCTION
	//spawn location only gets set at begin of level so movement should be active
	if (auto pMovement = m_pParentObject->GetComponent<MovementComponent>())
	{
		pMovement->SetEnabled(true);
	}
}