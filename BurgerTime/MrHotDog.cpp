#include "BurgerTimePCH.h"
#include "MrHotDog.h"
#include "GameObject.h"
#include "IngredientComponent.h"
#include "SpriteManagerComponent.h"
#include "Enums.h"

dae::MrHotDog::MrHotDog(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
{
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
				pSpriteManager->PlaySprite(static_cast<uint32_t>(SpriteIndices::Death), false);
			}
		}
	}
}
