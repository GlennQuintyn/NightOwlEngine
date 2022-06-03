#pragma once
#include <BaseComponent.h>
#include <Observer.h>
#include "EnemyControllerComponent.h"
#include "Enums.h"

namespace dae
{
	class GameObject;

	class MrPickle final : public BaseComponent, public Observer
	{
	public:
		MrPickle(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		MrPickle(std::nullptr_t) = delete;

		~MrPickle() = default;

		void SetDeathDuration(float duration) { m_DeathDurationLength = duration; };
		//location to (re)spawn at and the direction the AI should walk in initially when (re)spawning
		void SetRespawnPosAndWalkDirection(float x, float y, EnemyControllerComponent::MovementState direction);

		//when the enemy is either dead, peppered or wating to move (respawn delay) he is safe to touch
		bool IsHostile() { return m_State == EnemyState::Moving; };

		void Notify(GameObject* pObject, int event) override;

		void LateInit() override {};
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override {};

		MrPickle(const MrPickle& other) = delete;
		MrPickle(MrPickle&& other) = delete;
		MrPickle& operator=(const MrPickle& other) = delete;
		MrPickle& operator=(MrPickle&& other) = delete;


	private:
		//places enemy ofscreen so the collider logic can reset
		void PlaceOffScreen();

		GameObject* m_pParentObject;
		EnemyControllerComponent* m_pEnemyController;
		glm::vec2 m_RespawnPos;
		EnemyState m_State;
		EnemyControllerComponent::MovementState m_SpawnWalkDirection;
		float m_DeathDurationLength;
		float m_RespawnDelay;
		float m_DurationLeft;
		bool m_ResetInNextUpdate;
	};
}

/*

namespace dae
{
	class GameObject;

	class MrEgg final : public BaseComponent, public Observer
	{
	public:
		MrEgg(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		MrEgg(std::nullptr_t) = delete;

		~MrEgg() = default;

		void SetDeathDuration(float duration) { m_DeathDurationLength = duration; };
		//location to (re)spawn at and the direction the AI should walk in initially when (re)spawning
		void SetRespawnPosAndWalkDirection(float x, float y, EnemyControllerComponent::MovementState direction);

		//when the enemy is either dead, peppered or wating to move (respawn delay) he is safe to touch
		bool IsHostile() { return m_State == EnemyState::Moving; };

		void Notify(GameObject* pObject, int event) override;

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override;

		MrEgg(const MrEgg& other) = delete;
		MrEgg(MrEgg&& other) = delete;
		MrEgg& operator=(const MrEgg& other) = delete;
		MrEgg& operator=(MrEgg&& other) = delete;


	private:
		//places enemy ofscreen so the collider logic can reset
		void PlaceOffScreen();

		GameObject* m_pParentObject;
		EnemyControllerComponent* m_pEnemyController;
		glm::vec2 m_RespawnPos;
		EnemyState m_State;
		EnemyControllerComponent::MovementState m_SpawnWalkDirection;
		float m_DeathDurationLength;
		float m_RespawnDelay;
		float m_DurationLeft;
		bool m_ResetInNextUpdate;
	};
}
*/