#pragma once
#include <BaseComponent.h>
#include <Observer.h>
#include <Subject.h>
#include "Enums.h"

namespace dae
{
	class GameObject;
	class ScoreComponent;

	class LevelManager final : public BaseComponent, public Observer
	{
	public:
		LevelManager(GameObject* pParentObject);
		~LevelManager() = default;

		//make sure that component has a parent that isn't nullptr
		LevelManager(std::nullptr_t) = delete;

		void LateInit() override {};
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override;

		Subject& GetSubject() { return m_Subject; }

		void SetGameMode(GameMode gameMode) { m_GameMode = gameMode; };
		void SetEnemyPlayer(GameObject* pEnemyPlayer) { m_pEnemyPlayer = pEnemyPlayer; };
		void SetNormalPlayer(GameObject* pNormalPlayer) { m_pNormalPlayer = pNormalPlayer; };
		void SetCoopPlayer(GameObject* pCoopPlayer) { m_pCoopPlayer = pCoopPlayer; };
		void SetPepperObject(GameObject* pPepperObject) { m_pPepperObject = pPepperObject; };
		void SetScoreComponent(ScoreComponent* pScoreCmpt) { m_pScoreCmpt = pScoreCmpt; };
		void AddEnemy(GameObject* pEnemy);

		void Notify(GameObject* pObject, int event) override;
		void SetMaxFullPlateCount(int count) { m_MaxPlateFullCount = count; };
		void SetLastSceneIndex(int index) { m_LastSceneIndex = index; };

		void RestartCurrentLevel(bool withDelay = false);
		void ReturnToMainMenu(bool withDelay = false);
		void AdvanceToNextLevel(bool withDelay = false);

		LevelManager(const LevelManager& other) = delete;
		LevelManager(LevelManager&& other) = delete;
		LevelManager& operator=(const LevelManager& other) = delete;
		LevelManager& operator=(LevelManager&& other) = delete;

	private:
		enum class NextAction
		{
			Nothing,
			ResetEnemiesAndPlayer,
			RestartCurrentLevel,
			ReturnToMainMenu,
			AdvanceToNextLevel
		};

		void DisabledAll();

		Subject m_Subject;

		//depending on the game mode it should load in or not loading certain enemies
		std::vector<GameObject*> m_pEnemies;
		GameObject* m_pEnemyPlayer;
		GameObject* m_pNormalPlayer;
		GameObject* m_pCoopPlayer;
		GameObject* m_pPepperObject;
		GameObject* m_pParentObject;
		ScoreComponent* m_pScoreCmpt;

		NextAction m_NextAction{};
		GameMode m_GameMode;
		int m_PlateFullCount;
		int m_MaxPlateFullCount;
		int m_LastSceneIndex;
		float m_Delay;
		float m_TimeLeft{};
		bool m_TimerStarted{};
	};
}