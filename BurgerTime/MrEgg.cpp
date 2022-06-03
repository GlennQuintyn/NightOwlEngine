#include "BurgerTimePCH.h"
#include "MrEgg.h"
#include <GameObject.h>
#include <SpriteManagerComponent.h>
#include "IngredientComponent.h"
#include "PepperComponent.h"

dae::MrEgg::MrEgg(GameObject* pParentObject)
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

void dae::MrEgg::SetRespawnPosAndWalkDirection(float x, float y, EnemyControllerComponent::MovementState direction)
{
	x;
	y;
	direction;
}

void dae::MrEgg::Notify(GameObject* pObject, int event)
{
	pObject;
	event;
}

void dae::MrEgg::LateInit()
{
}

void dae::MrEgg::Update()
{
}

void dae::MrEgg::Reset()
{
}

void dae::MrEgg::PlaceOffScreen()
{
}
