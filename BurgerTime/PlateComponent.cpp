#include "BurgerTimePCH.h"
#include "PlateComponent.h"
#include "IngredientComponent.h"
#include "GameObject.h"
#include "RectColliderComponent.h"

dae::PlateComponent::PlateComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_pCollidermpt{ nullptr }
{
}

void dae::PlateComponent::LateInit()
{
	m_pCollidermpt = m_pParentObject->GetComponent<RectColliderComponent>();
	if (!m_pCollidermpt)
		Logger::GetInstance().LogWarning("PLATECOMPONENT:\tNO COLLIDER COMPONENT WAS FOUND!");
}

void dae::PlateComponent::Notify(GameObject* pObject, int event)
{
	if (auto pIngredient = pObject->GetComponent<IngredientComponent>())
	{
		if (event == 0)
		{
			auto yes = pObject->GetComponent<RectColliderComponent>();
			auto& colliderBox = m_pCollidermpt->GetRectangle();
			//increase the collider box size
			m_pCollidermpt->SetRectangle({ colliderBox.x, colliderBox.y - yes->GetRectangle().h, colliderBox.w, colliderBox.h + yes->GetRectangle().h });
		}
	}
}
