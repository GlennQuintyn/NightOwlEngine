#pragma once
#include "Command.h"
#include "GameObject.h"
#include "TimeSingleton.h"

namespace dae
{
	int g_MovementSpeedHorizontal{ 100 };
	int g_MovementSpeedVertical{ 50 };

	class WalkRightCommand final : public Command
	{
	public:
		void SetPlayer(GameObject* p) { m_pObject = p; };
	private:
		void Execute() override
		{
			auto& pos{ m_pObject->GetLoacalPosition() };
			m_pObject->SetLocalPosition(pos.x + (g_MovementSpeedHorizontal * Time::GetInstance().GetDeltaT()), pos.y);
		};

		GameObject* m_pObject{};//example code
	};

	class WalkDownCommand final : public Command
	{
	public:
		void SetPlayer(GameObject* p) { m_pObject = p; };
	private:
		void Execute() override
		{
			auto& pos{ m_pObject->GetLoacalPosition() };
			m_pObject->SetLocalPosition(pos.x, pos.y + (g_MovementSpeedVertical * Time::GetInstance().GetDeltaT()));
		};
		GameObject* m_pObject{};//example code
	};

	class WalkLeftCommand final : public Command
	{
	public:
		void SetPlayer(GameObject* p) { m_pObject = p; };
	private:
		void Execute() override
		{
			auto& pos{ m_pObject->GetLoacalPosition() };
			m_pObject->SetLocalPosition(pos.x - (g_MovementSpeedHorizontal * Time::GetInstance().GetDeltaT()), pos.y);
		};
		GameObject* m_pObject{};//example code
	};

	class WalkUpCommand final : public Command
	{
	public:
		void SetPlayer(GameObject* p) { m_pObject = p; };
	private:
		void Execute() override
		{
			auto& pos{ m_pObject->GetLoacalPosition() };
			m_pObject->SetLocalPosition(pos.x, pos.y - (g_MovementSpeedVertical * Time::GetInstance().GetDeltaT()));
		};
		GameObject* m_pObject{};//example code
	};
}