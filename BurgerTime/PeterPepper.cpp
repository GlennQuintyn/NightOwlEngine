#include "BurgerTimePCH.h"
#include "PeterPepper.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Subject.h"
#include "Enums.h"
#include "LadderComponent.h"
#include "RectColliderComponent.h"

using namespace dae;

PeterPepper::PeterPepper(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_IsOnLadder{ false }
	, m_DeahtButton{ PCController::ControllerButton::Button_Triangle }
	, m_ObjFellButton{ PCController::ControllerButton::Button_R_SHOULDER }
	, m_EnemyPepperedButton{ PCController::ControllerButton::Button_R_THUMB }
{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("FPSComponent:\tPARENT OBJECT WAS NOT GIVEN!");

	m_pSubject = std::make_unique<Subject>();
}

void PeterPepper::LateInit()
{
	//if (!m_pParentObject->GetComponent<Subject>())
	//	Logger::GetInstance().LogError("PeterPepperComponent:\tSUBJECTCOMPONENT WAS NOT GIVEN!");
	if (auto pCollidercmpt = m_pParentObject->GetComponent<RectColliderComponent>())
	{
		m_Size.x = pCollidercmpt->GetRectangle().w;
		m_Size.y = pCollidercmpt->GetRectangle().h;
	}
}

void PeterPepper::Update()
{
	if (InputManager::GetInstance().IsPressedThisFrame(m_DeahtButton))
	{
		m_pSubject->Notify(m_pParentObject, int(Events::PeterPepper_Died));

		//m_pParentObject->GetComponent<Subject>()->Notify(m_pParentObject, int(Events::PeterPepper_Died));
	}
	if (InputManager::GetInstance().IsPressedThisFrame(m_ObjFellButton))
	{
		m_pSubject->Notify(m_pParentObject, int(Events::Item_Fell));
		//m_pParentObject->GetComponent<Subject>()->Notify(m_pParentObject, int(Events::Item_Fell));
	}
	if (InputManager::GetInstance().IsPressedThisFrame(m_EnemyPepperedButton))
	{
		m_pSubject->Notify(m_pParentObject, int(Events::Enemy_Peppered));
		//m_pParentObject->GetComponent<Subject>()->Notify(m_pParentObject, int(Events::Enemy_Peppered));
	}
}

void dae::PeterPepper::Notify(GameObject* pObject, int event)
{
	//check if the object has a ladder component
	if (auto pLaddercmp = pObject->GetComponent<LadderComponent>())
	{
		//if the event is a 0 that means it entered a ladder and if its 0 it exited one
		switch (event)
		{
		case 0:
		{
			m_IsOnLadder = true;
			//set petterpepper on the center of the ladder
			auto& ladderPos = pObject->GetWorldPosition();
			auto& peterPos = m_pParentObject->GetWorldPosition();
			m_pParentObject->SetLocalPosition(ladderPos.x - (m_Size.x / 2.f), peterPos.y);
			break;
		}
		case 1:
			m_IsOnLadder = false;
			break;
		default:
			Logger::GetInstance().LogError("PetterPeppber: invalid switch state! reached");
			break;
		}
	}


}

void PeterPepper::SetDeathButton(PCController::ControllerButton deahtButton)
{
	m_DeahtButton = deahtButton;
}

void PeterPepper::SetObjectFellButton(PCController::ControllerButton objFellButton)
{
	m_ObjFellButton = objFellButton;
}

void PeterPepper::SetPepperEnemyButton(PCController::ControllerButton enemyPepperedButton)
{
	m_EnemyPepperedButton = enemyPepperedButton;
}
