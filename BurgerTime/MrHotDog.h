#pragma once
#include <BaseComponent.h>
#include <Observer.h>
#include "EnemyControllerComponent.h"
#include "Enums.h"

namespace dae
{
	class GameObject;
	class IngredientComponent;

	class MrHotDog final : public BaseComponent, public Observer
	{
	public:
		MrHotDog(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		MrHotDog(std::nullptr_t) = delete;

		~MrHotDog() = default;

		void Notify(GameObject* pObject, int event) override;

		void SetDeathDuration(float duration) { m_DeathDurationLength = duration; };
		//location to (re)spawn at and the direction the AI should walk in initially when (re)spawning
		void SetRespawnPosAndWalkDirection(float x, float y, EnemyControllerComponent::MovementState direction);

		void SetControlledByHuman(bool state) { m_ControlledByHuman = state; };

		//when the enemy is either dead, peppered or wating to move (respawn delay) he is safe to touch
		bool IsHostile() { return m_State == EnemyState::Moving; };

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override;

		MrHotDog(const MrHotDog& other) = delete;
		MrHotDog(MrHotDog&& other) = delete;
		MrHotDog& operator=(const MrHotDog& other) = delete;
		MrHotDog& operator=(MrHotDog&& other) = delete;

	private:
		//places enemy ofscreen so the collider logic can reset
		void PlaceOffScreen();

		GameObject* m_pParentObject;
		IngredientComponent* m_pIngredientWalkingOn;
		EnemyControllerComponent* m_pEnemyController;
		
		MovementComponent* m_pMovementCmpt;

		glm::vec2 m_RespawnPos;
		EnemyState m_State;
		EnemyControllerComponent::MovementState m_SpawnWalkDirection;
		float m_DeathDurationLength;
		float m_RespawnDelay;
		float m_DurationLeft;
		bool m_ResetInNextUpdate;
		bool m_ControlledByHuman;
	};
}
