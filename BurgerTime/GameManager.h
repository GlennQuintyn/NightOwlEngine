#pragma once
#include <BaseComponent.h>
#include <Observer.h>

namespace dae
{
	class GameObject;

	class GameManager final : public BaseComponent, public Observer
	{
	public:
		GameManager(GameObject* pParentObject);
		~GameManager() = default;

		//make sure that component has a parent that isn't nullptr
		GameManager(std::nullptr_t) = delete;

		void LateInit() override {};
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override {};

		void Notify(GameObject* pObject, int event) override;

		void RestartCurrentLevel(bool withDelay = false);
		void ReturnToMainMenu(bool withDelay = false);

		GameManager(const GameManager& other) = delete;
		GameManager(GameManager&& other) = delete;
		GameManager& operator=(const GameManager& other) = delete;
		GameManager& operator=(GameManager&& other) = delete;

	private:
		enum class NextAction
		{
			Nothing,
			RestartCurrentLevel,
			ReturnToMainMenu
		};
		GameObject* m_pParentObject;

		NextAction m_NextAction{};
		float m_ResetDelay{ 4.5f };
		float m_TimeLeft{};
		bool m_TimerStarted{};
	};
}