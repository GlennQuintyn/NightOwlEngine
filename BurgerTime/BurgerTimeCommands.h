#pragma once
#include "Command.h"
#include "GameObject.h"
#include "TimeSingleton.h"
#include "MovementComponent.h"
#include "RectColliderComponent.h"
#include "SpriteManagerComponent.h"
#include "Enums.h"

namespace dae
{
	int g_MovementSpeedHorizontal{ 100 };
	int g_MovementSpeedVertical{ 75 };//50

	class WalkLeftCommand final : public Command
	{
	public:
		void SetPlayer(GameObject* p)
		{
			m_pObject = p;
			m_pMovementCmpt = m_pObject->GetComponent<MovementComponent>();
			if (!m_pMovementCmpt)
			{
				Logger::GetInstance().LogError("Commands: no movementcomponent was found!");
			}
			auto pCollider = m_pObject->GetComponent<RectColliderComponent>();
			if (!pCollider)
			{
				Logger::GetInstance().LogError("Commands: no RectColliderComponent was found!");
				return;
			}
			m_Size.x = pCollider->GetRectangle().w;
			m_Size.y = pCollider->GetRectangle().h;

			m_pSpriteManagerCmpt = m_pObject->GetComponent<SpriteManagerComponent>();
		};
	private:
		void Execute() override
		{
			if (m_pMovementCmpt->CanGoLeft())
			{
				auto pPlatform = m_pMovementCmpt->GetTouchingPlatformLeft();
				if (!pPlatform)
					return;

				auto& PlatformPos = pPlatform->GetWorldPosition();
				auto pPlarformCollider = pPlatform->GetComponent<RectColliderComponent>();
				auto& pos{ m_pObject->GetWorldPosition() };

				m_pObject->SetLocalPosition(pos.x - (g_MovementSpeedHorizontal * Time::GetInstance().GetDeltaT()), PlatformPos.y + (pPlarformCollider->GetRectangle().h) - m_Size.y);

				if (m_pSpriteManagerCmpt)
					m_pSpriteManagerCmpt->PlaySprite(static_cast<uint32_t>(SpriteIndices::WalkLeft));
			}
		};
		glm::ivec2 m_Size{};
		GameObject* m_pObject{};//example code
		MovementComponent* m_pMovementCmpt{};
		SpriteManagerComponent* m_pSpriteManagerCmpt{};
	};

	class WalkRightCommand final : public Command
	{
	public:
		void SetPlayer(GameObject* p)
		{
			m_pObject = p;
			m_pMovementCmpt = m_pObject->GetComponent<MovementComponent>();
			if (!m_pMovementCmpt)
			{
				Logger::GetInstance().LogError("Commands: no movementcomponent was found!");
			}
			auto pCollider = m_pObject->GetComponent<RectColliderComponent>();
			if (!pCollider)
			{
				Logger::GetInstance().LogError("Commands: no RectColliderComponent was found!");
				return;
			}
			m_Size.x = pCollider->GetRectangle().w;
			m_Size.y = pCollider->GetRectangle().h;

			m_pSpriteManagerCmpt = m_pObject->GetComponent<SpriteManagerComponent>();
		};
	private:
		void Execute() override
		{
			if (m_pMovementCmpt->CanGoRight())
			{
				auto pPlatform = m_pMovementCmpt->GetTouchingPlatformRight();
				if (!pPlatform)
					return;

				auto& PlatformPos = pPlatform->GetWorldPosition();
				auto pPlarformCollider = pPlatform->GetComponent<RectColliderComponent>();
				auto& pos{ m_pObject->GetWorldPosition() };

				m_pObject->SetLocalPosition(pos.x + (g_MovementSpeedHorizontal * Time::GetInstance().GetDeltaT()), PlatformPos.y + (pPlarformCollider->GetRectangle().h) - m_Size.y);

				if (m_pSpriteManagerCmpt)
					m_pSpriteManagerCmpt->PlaySprite(static_cast<uint32_t>(SpriteIndices::WalkRight));
			}
		};
		glm::ivec2 m_Size{};
		GameObject* m_pObject{};//example code
		MovementComponent* m_pMovementCmpt{};
		SpriteManagerComponent* m_pSpriteManagerCmpt{};
	};

	class WalkUpCommand final : public Command
	{
	public:
		void SetPlayer(GameObject* p)
		{
			m_pObject = p;
			m_pMovementCmpt = m_pObject->GetComponent<MovementComponent>();
			if (!m_pMovementCmpt)
			{
				Logger::GetInstance().LogError("Commands: no movementcomponent was found!");
			}
			auto pCollider = m_pObject->GetComponent<RectColliderComponent>();
			if (!pCollider)
			{
				Logger::GetInstance().LogError("Commands: no RectColliderComponent was found!");
				return;
			}
			m_Size.x = pCollider->GetRectangle().w;
			m_Size.y = pCollider->GetRectangle().h;

			m_pSpriteManagerCmpt = m_pObject->GetComponent<SpriteManagerComponent>();
		};
	private:
		void Execute() override
		{
			if (m_pMovementCmpt->CanGoUp())
			{
				auto pLadder = m_pMovementCmpt->GetTouchingLadderUp();
				if (!pLadder)
					return;

				auto& ladderPos = pLadder->GetWorldPosition();
				auto pLadderCollider = pLadder->GetComponent<RectColliderComponent>();
				auto& pos{ m_pObject->GetWorldPosition() };

				m_pObject->SetLocalPosition(ladderPos.x + (pLadderCollider->GetRectangle().w / 2.f) - (m_Size.x / 2.f), pos.y - (g_MovementSpeedVertical * Time::GetInstance().GetDeltaT()));

				if (m_pSpriteManagerCmpt)
					m_pSpriteManagerCmpt->PlaySprite(static_cast<uint32_t>(SpriteIndices::WalkUp));
			}
		};
		glm::ivec2 m_Size{};
		GameObject* m_pObject{};//example code
		MovementComponent* m_pMovementCmpt{};
		SpriteManagerComponent* m_pSpriteManagerCmpt{};
	};

	class WalkDownCommand final : public Command
	{
	public:
		void SetPlayer(GameObject* p)
		{
			m_pObject = p;
			m_pMovementCmpt = m_pObject->GetComponent<MovementComponent>();
			if (!m_pMovementCmpt)
			{
				Logger::GetInstance().LogError("Commands: no movementcomponent was found!");
			}
			auto pCollider = m_pObject->GetComponent<RectColliderComponent>();
			if (!pCollider)
			{
				Logger::GetInstance().LogError("Commands: no RectColliderComponent was found!");
				return;
			}
			m_Size.x = pCollider->GetRectangle().w;
			m_Size.y = pCollider->GetRectangle().h;

			m_pSpriteManagerCmpt = m_pObject->GetComponent<SpriteManagerComponent>();
		};
	private:
		void Execute() override
		{
			if (m_pMovementCmpt->CanGoDown())
			{
				auto pLadder = m_pMovementCmpt->GetTouchingLadderDown();
				if (!pLadder)
					return;

				auto& ladderPos = pLadder->GetWorldPosition();
				auto pLadderCollider = pLadder->GetComponent<RectColliderComponent>();
				auto& pos{ m_pObject->GetWorldPosition() };
				m_pObject->SetLocalPosition(ladderPos.x + (pLadderCollider->GetRectangle().w / 2.f) - (m_Size.x / 2.f), pos.y + (g_MovementSpeedVertical * Time::GetInstance().GetDeltaT()));

				if (m_pSpriteManagerCmpt)
					m_pSpriteManagerCmpt->PlaySprite(static_cast<uint32_t>(SpriteIndices::WalkDown));
			}
		};
		glm::ivec2 m_Size{};
		GameObject* m_pObject{};//example code
		MovementComponent* m_pMovementCmpt{};
		SpriteManagerComponent* m_pSpriteManagerCmpt{};
	};

	class ThrowPepperCommand final : public Command
	{
	public:
		void SetPepper(GameObject* p)
		{
			m_pPepperObject = p;

			auto pCollider = m_pPepperObject->GetComponent<RectColliderComponent>();
			if (!pCollider)
			{
				Logger::GetInstance().LogError("ThrowPepperCommand: no RectColliderComponent was found!");
				return;
			}
			m_Size.x = pCollider->GetRectangle().w;
			m_Size.y = pCollider->GetRectangle().h;
		}
		void SetPlayer(GameObject* p)
		{
			m_pPlayerObject = p;
		}
	private:
		void Execute() override
		{
			auto pPlayerMovementcmpt = m_pPlayerObject->GetComponent<MovementComponent>();
			auto pPlayerCollidercmpt = m_pPlayerObject->GetComponent<RectColliderComponent>();
			auto pPepperSpriteManagercmpt = m_pPepperObject->GetComponent<SpriteManagerComponent>();

			if (pPlayerMovementcmpt && pPlayerCollidercmpt)
			{
				auto& playerPos = m_pPlayerObject->GetWorldPosition();
				auto pPepperCmpt = m_pPepperObject->GetComponent<PepperComponent>();

				WalkingDirection direction = pPlayerMovementcmpt->GetLastWalkingDirection();
				switch (direction)
				{
				case dae::WalkingDirection::Left:
					m_pPepperObject->SetLocalPosition(playerPos.x - m_Size.x, playerPos.y);
					pPepperSpriteManagercmpt->PlaySprite(0, SpriteManagerComponent::SpritePlayType::PlayOnce);
					break;
				case dae::WalkingDirection::Right:
					pPepperSpriteManagercmpt->PlaySprite(1, SpriteManagerComponent::SpritePlayType::PlayOnce);
					m_pPepperObject->SetLocalPosition(playerPos.x + pPlayerCollidercmpt->GetRectangle().w, playerPos.y);
					break;
				case dae::WalkingDirection::Up:
					pPepperSpriteManagercmpt->PlaySprite(2, SpriteManagerComponent::SpritePlayType::PlayOnce);
					m_pPepperObject->SetLocalPosition(playerPos.x, playerPos.y - m_Size.y);
					break;
				case dae::WalkingDirection::Down:
					pPepperSpriteManagercmpt->PlaySprite(3, SpriteManagerComponent::SpritePlayType::PlayOnce);
					m_pPepperObject->SetLocalPosition(playerPos.x, playerPos.y + pPlayerCollidercmpt->GetRectangle().h);
					break;
				default:
					Logger::GetInstance().LogError("ThrowPepperCommand: invalid switch state reached!");
					break;
				}

				pPepperCmpt->Thrown();
			}
		}
		GameObject* m_pPepperObject{};
		GameObject* m_pPlayerObject{};
		glm::ivec2 m_Size{};
	};
}