#pragma once
#include <Command.h>
#include <GameObject.h>
#include <RectColliderComponent.h>
#include <SpriteManagerComponent.h>
#include "Enums.h"
#include "MovementComponent.h"
#include "MainMenuComponent.h"
#include "PepperComponent.h"
#include "PepperCountComponent.h"

namespace dae
{
	const int g_MovementSpeedHorizontal{ 100 };
	const int g_MovementSpeedVertical{ 75 };//50

	//TODO: fix the player centering on the ladder code to also use the x and y offsets

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
			//if the object is not enabled it shouldn't execute the commands
			if (!m_pObject->IsEnabled())
				return;

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
			//if the object is not enabled it shouldn't execute the commands
			if (!m_pObject->IsEnabled())
				return;

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
			//if the object is not enabled it shouldn't execute the commands
			if (!m_pObject->IsEnabled())
				return;

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
			//if the object is not enabled it shouldn't execute the commands
			if (!m_pObject->IsEnabled())
				return;

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
		void SetPepperCountComponent(PepperCountComponent* p)
		{
			m_pPepperCountCmpt = p;
		}

	private:
		void Execute() override
		{
			//if the object is not enabled it shouldn't execute the commands
			if (!m_pPepperObject->IsEnabled())
				return;

			//if there is no pepper left to throw cancel the pepper throwing
			if (!m_pPepperCountCmpt->UsePepper())
				return;

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

				if (pPepperCmpt)
					pPepperCmpt->Thrown();
			}
		}
		GameObject* m_pPepperObject{};
		GameObject* m_pPlayerObject{};
		PepperCountComponent* m_pPepperCountCmpt{};
		glm::ivec2 m_Size{};
	};

#pragma region MainMenuCommands

	class NextButtonCommand final : public Command
	{
	public:
		void SetMainMenuCmpt(MainMenuComponent* pMainMenu)
		{
			if (pMainMenu)
				m_pMainMenu = pMainMenu;
		}
	private:
		void Execute() override
		{
			//only executee main menu commands when the active scene is the main menu
			if (SceneManager::GetInstance().GetActiveScene() == 0)
				m_pMainMenu->SelectNextButton();
		}

		MainMenuComponent* m_pMainMenu{};
	};

	class PreviousButtonCommand final : public Command
	{
	public:
		void SetMainMenuCmpt(MainMenuComponent* pMainMenu)
		{
			if (pMainMenu)
				m_pMainMenu = pMainMenu;
		}

	private:
		void Execute() override
		{
			//only executee main menu commands when the active scene is the main menu
			if (SceneManager::GetInstance().GetActiveScene() == 0)
				m_pMainMenu->SelectPreviousButton();
		}

		MainMenuComponent* m_pMainMenu{};
	};

	class MouseButtonCommand final : public Command
	{
	public:
		void SetMainMenuCmpt(MainMenuComponent* pMainMenu)
		{
			if (pMainMenu)
				m_pMainMenu = pMainMenu;
		}

	private:
		void Execute() override
		{
			//only executee main menu commands when the active scene is the main menu
			if (SceneManager::GetInstance().GetActiveScene() == 0)
				m_pMainMenu->CheckIfMouseInsideButton();
		}

		MainMenuComponent* m_pMainMenu{};
	};

	class PressSelectedButtonCommand final : public Command
	{
	public:
		void SetMainMenuCmpt(MainMenuComponent* pMainMenu)
		{
			if (pMainMenu)
				m_pMainMenu = pMainMenu;
		}

	private:
		void Execute() override
		{
			//only executee main menu commands when the active scene is the main menu
			if (SceneManager::GetInstance().GetActiveScene() == 0)
				m_pMainMenu->PressSelectedButton();
		}

		MainMenuComponent* m_pMainMenu{};
	};
#pragma endregion
}