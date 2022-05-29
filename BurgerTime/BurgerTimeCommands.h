#pragma once
#include "Command.h"
#include "GameObject.h"
#include "TimeSingleton.h"
#include "MovementComponent.h"
#include "RectColliderComponent.h"

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
		};
	private:
		void Execute() override
		{
			if (m_pMovementCmpt->CanGoLeft())
			{
				auto pPlatform = m_pMovementCmpt->GetTouchingPlatformLeft();
				auto& PlatformPos = pPlatform->GetWorldPosition();
				auto pPlarformCollider = pPlatform->GetComponent<RectColliderComponent>();
				auto& pos{ m_pObject->GetLoacalPosition() };

				//m_pObject->SetLocalPosition(pos.x + (g_MovementSpeedHorizontal * Time::GetInstance().GetDeltaT()), pos.y + (pPlarformCollider->GetRectangle().h) - m_Size.y);
				m_pObject->SetLocalPosition(pos.x - (g_MovementSpeedHorizontal * Time::GetInstance().GetDeltaT()), PlatformPos.y + (pPlarformCollider->GetRectangle().h) - m_Size.y);
			}
			//if (auto pPetterPepper = m_pObject->GetComponent<PeterPepper>())
			//{
			//	if (pPetterPepper->IsOnLadder())
			//	{
			//		Logger::GetInstance().LogInfo("cannot walk LEFT");
			//		return;
			//	}
			//}
		};
		GameObject* m_pObject{};//example code
		glm::ivec2 m_Size;
		MovementComponent* m_pMovementCmpt;
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
		};
	private:
		void Execute() override
		{
			if (m_pMovementCmpt->CanGoRight())
			{
				auto pPlatform = m_pMovementCmpt->GetTouchingPlatformRight();
				auto& PlatformPos = pPlatform->GetWorldPosition();
				auto pPlarformCollider = pPlatform->GetComponent<RectColliderComponent>();
				auto& pos{ m_pObject->GetLoacalPosition() };
				m_pObject->SetLocalPosition(pos.x + (g_MovementSpeedHorizontal * Time::GetInstance().GetDeltaT()), PlatformPos.y + (pPlarformCollider->GetRectangle().h) - m_Size.y);
			}
			//if (auto pPetterPepper = m_pObject->GetComponent<PeterPepper>())
			//{
			//	if (pPetterPepper->IsOnLadder())
			//	{
			//		Logger::GetInstance().LogInfo("cannot walk RIGHT");
			//		return;
			//	}
			//}
		};
		GameObject* m_pObject{};//example code
		glm::ivec2 m_Size;
		MovementComponent* m_pMovementCmpt;
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
		};
	private:
		void Execute() override
		{
			if (m_pMovementCmpt->CanGoUp())
			{
				//auto& PlatformPos = pObject->GetWorldPosition();
				//auto& peterPos = m_pParentObject->GetWorldPosition();
				//m_pParentObject->SetLocalPosition(PlatformPos.x - (m_Size.x / 2.f), peterPos.y);
				//m_pObject->SetLocalPosition(pos.x, pos.y - (g_MovementSpeedVertical * Time::GetInstance().GetDeltaT()));

				auto pLadder = m_pMovementCmpt->GetTouchingLadderUp();
				auto& ladderPos = pLadder->GetWorldPosition();
				auto pLadderCollider = pLadder->GetComponent<RectColliderComponent>();
				auto& pos{ m_pObject->GetLoacalPosition() };

				m_pObject->SetLocalPosition(ladderPos.x + (pLadderCollider->GetRectangle().w / 2.f) - (m_Size.x / 2.f), pos.y - (g_MovementSpeedVertical * Time::GetInstance().GetDeltaT()));
			}
		};
		GameObject* m_pObject{};//example code
		glm::ivec2 m_Size;
		MovementComponent* m_pMovementCmpt;
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
		};
	private:
		void Execute() override
		{
			if (m_pMovementCmpt->CanGoDown())
			{
				auto pLadder = m_pMovementCmpt->GetTouchingLadderDown();
				auto& ladderPos = pLadder->GetWorldPosition();
				auto pLadderCollider = pLadder->GetComponent<RectColliderComponent>();
				auto& pos{ m_pObject->GetLoacalPosition() };
				m_pObject->SetLocalPosition(ladderPos.x + (pLadderCollider->GetRectangle().w / 2.f) - (m_Size.x / 2.f), pos.y + (g_MovementSpeedVertical * Time::GetInstance().GetDeltaT()));
			}
		};
		GameObject* m_pObject{};//example code
		glm::ivec2 m_Size;
		MovementComponent* m_pMovementCmpt;
	};
}