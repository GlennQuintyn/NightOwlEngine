#include "NightOwlEnginePCH.h"
#include "PeterPepper.h"
#include "InputManager.h"
#include "GameObject.h"
#include "SubjectComponent.h"
#include "Enums.h"

using namespace dae;

dae::PeterPepper::PeterPepper(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_DeahtButton{ PCController::ControllerButton::Button_Triangle }
	, m_ObjFellButton{ PCController::ControllerButton::Button_R_SHOULDER }
	, m_EnemyPepperedButton{ PCController::ControllerButton::Button_R_THUMB }

{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("FPSComponent:\tPARENT OBJECT WAS NOT GIVEN!");
}

void dae::PeterPepper::LateInit()
{
	if (!m_pParentObject->GetComponent<SubjectComponent>())
		Logger::GetInstance().LogError("PeterPepperComponent:\tSUBJECTCOMPONENT WAS NOT GIVEN!");
}

void PeterPepper::Update()
{
	if (InputManager::GetInstance().IsPressedThisFrame(m_DeahtButton))
	{
		m_pParentObject->GetComponent<SubjectComponent>()->Notify(m_pParentObject, int(Events::PeterPepper_Died));
	}
	if (InputManager::GetInstance().IsPressedThisFrame(m_ObjFellButton))
	{
		m_pParentObject->GetComponent<SubjectComponent>()->Notify(m_pParentObject, int(Events::Item_Fell));
	}
	if (InputManager::GetInstance().IsPressedThisFrame(m_EnemyPepperedButton))
	{
		m_pParentObject->GetComponent<SubjectComponent>()->Notify(m_pParentObject, int(Events::Enemy_Peppered));
	}
}

void dae::PeterPepper::SetDeathButton(PCController::ControllerButton deahtButton)
{
	m_DeahtButton = deahtButton;
}

void dae::PeterPepper::SetObjectFellButton(PCController::ControllerButton objFellButton)
{
	m_ObjFellButton = objFellButton;
}

void dae::PeterPepper::SetPepperEnemyButton(PCController::ControllerButton enemyPepperedButton)
{
	m_EnemyPepperedButton = enemyPepperedButton;
}
