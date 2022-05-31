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
}