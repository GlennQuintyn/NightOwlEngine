#pragma once
#include <BaseComponent.h>
#include <Observer.h>
#include <Subject.h>

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
		void Reset() override;

		Subject& GetSubject() { return m_Subject; }

		void Notify(GameObject* pObject, int event) override;
		void SetMaxFullPlateCount(int count) { m_MaxPlateFullCount = count; };
		void SetLastSceneIndex(int index) { m_LastSceneIndex = index; };

		void RestartCurrentLevel(bool withDelay = false);
		void ReturnToMainMenu(bool withDelay = false);
		void AdvanceToNextLevel(bool withDelay = false);

		GameManager(const GameManager& other) = delete;
		GameManager(GameManager&& other) = delete;
		GameManager& operator=(const GameManager& other) = delete;
		GameManager& operator=(GameManager&& other) = delete;

	private:
		enum class NextAction
		{
			Nothing,
			ResetEnemiesAndPlayer,
			RestartCurrentLevel,
			ReturnToMainMenu,
			AdvanceToNextLevel
		};

		Subject m_Subject;
		GameObject* m_pParentObject;
		NextAction m_NextAction{};
		int m_PlateFullCount;
		int m_MaxPlateFullCount;
		int m_LastSceneIndex;
		float m_Delay;
		float m_TimeLeft{};
		bool m_TimerStarted{};
	};
}