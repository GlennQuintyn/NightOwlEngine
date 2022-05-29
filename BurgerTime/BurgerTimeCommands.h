#pragma once
#include "Command.h"
#include "GameObject.h"
#include "TimeSingleton.h"
#include "MovementComponent.h"

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
		};
	private:
		void Execute() override
		{
			if (m_pMovementCmpt->CanGoLeft())
			{
				auto& pos{ m_pObject->GetLoacalPosition() };
				m_pObject->SetLocalPosition(pos.x - (g_MovementSpeedHorizontal * Time::GetInstance().GetDeltaT()), pos.y);
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
		};
	private:
		void Execute() override
		{
			if (m_pMovementCmpt->CanGoRight())
			{
				auto& pos{ m_pObject->GetLoacalPosition() };
				m_pObject->SetLocalPosition(pos.x + (g_MovementSpeedHorizontal * Time::GetInstance().GetDeltaT()), pos.y);
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
		};
	private:
		void Execute() override
		{
			if (m_pMovementCmpt->CanGoUp())
			{
				auto& pos{ m_pObject->GetLoacalPosition() };
				m_pObject->SetLocalPosition(pos.x, pos.y - (g_MovementSpeedVertical * Time::GetInstance().GetDeltaT()));
			}
		};
		GameObject* m_pObject{};//example code
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
		};
	private:
		void Execute() override
		{
			if (m_pMovementCmpt->CanGoDown())
			{
				auto& pos{ m_pObject->GetLoacalPosition() };
				m_pObject->SetLocalPosition(pos.x, pos.y + (g_MovementSpeedVertical * Time::GetInstance().GetDeltaT()));
			}
		};
		GameObject* m_pObject{};//example code
		MovementComponent* m_pMovementCmpt;
	};
}