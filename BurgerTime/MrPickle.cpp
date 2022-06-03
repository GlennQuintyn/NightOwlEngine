#include "BurgerTimePCH.h"
#include "MrPickle.h"
#include <GameObject.h>
#include <SpriteManagerComponent.h>
#include "IngredientComponent.h"
#include "PepperComponent.h"

dae::MrPickle::MrPickle(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
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

void dae::MrPickle::SetRespawnPosAndWalkDirection(float x, float y, EnemyControllerComponent::MovementState direction)
{
	x;
	y;
	direction;

}

void dae::MrPickle::Notify(GameObject* pObject, int event)
{
	pObject;
	event;
}

void dae::MrPickle::PlaceOffScreen()
{
}
