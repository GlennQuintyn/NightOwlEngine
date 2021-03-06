#pragma once
#include <BaseComponent.h>

namespace dae
{
	class GameObject;
	class MovementComponent;
	class SpriteManagerComponent;

	class EnemyControllerComponent final : public BaseComponent
	{
	public:
		enum class MovementState
		{
			LadderDone,
			Left,
			Right,
			PlatformDone,
			UpLadder,
			DownLadder,
			NoMovement,
			UsePrevious
		};

		EnemyControllerComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		EnemyControllerComponent(std::nullptr_t) = delete;

		~EnemyControllerComponent() = default;

		void SetMovement(bool enable, MovementState state = MovementState::UsePrevious);

		void SetPlayer1(GameObject* pPlayer1) { m_pPlayer1 = pPlayer1; };
		//set player 2 to nullptr to remove from AI path checking logic
		void SetPlayer2(GameObject* pPlayer2) { m_pPlayer2 = pPlayer2; };

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override {};

		EnemyControllerComponent(const EnemyControllerComponent& other) = delete;
		EnemyControllerComponent(EnemyControllerComponent&& other) = delete;
		EnemyControllerComponent& operator=(const EnemyControllerComponent& other) = delete;
		EnemyControllerComponent& operator=(EnemyControllerComponent&& other) = delete;

	private:
		void MoveLeft(const glm::vec2& pos);
		void MoveRigth(const glm::vec2& pos);
		void MoveUp(const glm::vec2& pos);
		void MoveDown(const glm::vec2& pos);

		glm::vec2 m_MovmentSpeed;
		glm::ivec2 m_Size;

		GameObject* m_pParentObject;
		GameObject* m_pPlayer1;
		GameObject* m_pPlayer2;
		MovementComponent* m_pMovementCmpt;
		SpriteManagerComponent* m_pSpriteManagerCmpt;
		MovementState m_State;
		MovementState m_PreviousState;
	};
}