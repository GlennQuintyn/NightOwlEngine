#include "BurgerTimePCH.h"
#include "LadderComponent.h"
#include "GameObject.h"
//#include "RectColliderComponent.h"

dae::LadderComponent::LadderComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
{
	//auto& colliderComponent = m_pParentObject->AddComponent<RectColliderComponent>();
	//colliderComponent.SetDebugMode(true);
}
