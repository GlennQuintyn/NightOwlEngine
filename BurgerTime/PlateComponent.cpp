#include "BurgerTimePCH.h"
#include "PlateComponent.h"
#include "IngredientComponent.h"
#include <GameObject.h>
#include <RectColliderComponent.h>
#include "Enums.h"

dae::PlateComponent::PlateComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_Subject{}
	, m_pCollidermpt{ nullptr }
	, m_IngredientsCount{ 0 }
	, m_MaxIngredientsCount{}
	, m_IntialCollider{}
{
}

void dae::PlateComponent::LateInit()
{
	m_pCollidermpt = m_pParentObject->GetComponent<RectColliderComponent>();
	if (!m_pCollidermpt)
	{
		Logger::GetInstance().LogWarning("PLATECOMPONENT:\tNO COLLIDER COMPONENT WAS FOUND!");
		return;
	}

	m_IntialCollider = m_pCollidermpt->GetRectangle();
}

void dae::PlateComponent::Notify(GameObject* pObject, int event)
{
	if (auto pIngredient = pObject->GetComponent<IngredientComponent>())
	{
		if (event == 0)
		{
			auto pCollider = pObject->GetComponent<RectColliderComponent>();
			auto& colliderBox = m_pCollidermpt->GetRectangle();
			//increase the collider box size
			m_pCollidermpt->SetRectangle({ colliderBox.x, colliderBox.y - pCollider->GetRectangle().h, colliderBox.w, colliderBox.h + pCollider->GetRectangle().h });
			++m_IngredientsCount;

			//Logger::GetInstance().LogInfo("Plate!");

			if (m_IngredientsCount >= m_MaxIngredientsCount)
			{
				m_Subject.Notify(m_pParentObject, static_cast<int>(Events::Plate_Full));
			}
		}
	}
}

void dae::PlateComponent::Reset()
{
	m_IngredientsCount = 0;
	//auto& colliderBox = m_pCollidermpt->GetRectangle();
	//reset the collider box to its orriginal size 
	m_pCollidermpt->SetRectangle(m_IntialCollider);
}