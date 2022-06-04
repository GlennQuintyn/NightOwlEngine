#pragma once
#include "Singleton.h"
#include "Scene.h"

namespace dae
{
	//TO: create only 1 active scene and a goto next scene or certain scene index

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string_view name);

		void LateInit();
		void Update();
		void FixedUpdate(float deltaT);
		void LateUpdate();
		void Render();
		void ResetActiveScene();

		~SceneManager() = default;

		void SetActiveScene(int index);
		const int GetActiveScene() const { return m_ActiveSceneIndex; };
		void GotoNextScene();
		void GotoPreviousScene();

		SceneManager(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager& operator=(SceneManager&& other) = delete;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::vector<std::unique_ptr<Scene>> m_pScenes;

		int m_ActiveSceneIndex{};
	};
}
