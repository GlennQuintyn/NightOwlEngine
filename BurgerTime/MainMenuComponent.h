#pragma once
#include <BaseComponent.h>
#include "ButtonComponent.h"
#include "Enums.h"

namespace dae
{
	class GameObject;
	class NightOwlEngine;
	class LevelManager;

	class MainMenuComponent final : public BaseComponent
	{
	public:
		MainMenuComponent(GameObject* pParentObject);
		~MainMenuComponent() = default;

		//make sure that component has a parent that isn't nullptr
		MainMenuComponent(std::nullptr_t) = delete;

		void AddButton(ButtonComponent* button) { m_pButtons.emplace_back(button); };
		void SelectNextButton();
		void SelectPreviousButton();
		void CheckIfMouseInsideButton();
		void SetEngine(NightOwlEngine* pEngine) { m_pEngine = pEngine; };
		void PressSelectedButton();

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override {};

		MainMenuComponent(const MainMenuComponent& other) = delete;
		MainMenuComponent(MainMenuComponent&& other) = delete;
		MainMenuComponent& operator=(const MainMenuComponent& other) = delete;
		MainMenuComponent& operator=(MainMenuComponent&& other) = delete;

	private:
		void LoadLevel1();
		void LoadLevel2();
		void LoadLevel3();

		std::vector<ButtonComponent*> m_pButtons;
		std::vector<LevelManager*> m_pLevelManagers;
		GameObject* m_pParentObject;
		NightOwlEngine* m_pEngine;
		int m_SelectedButtonIndex;
		GameMode m_GameMode;

		bool m_ScenesLoadedBefore;

		bool m_CoopLoadedBefore;
		bool m_VersusLoadedBefore;
	};
}