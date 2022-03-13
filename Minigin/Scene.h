#pragma once
//#include "SceneManager.h"

namespace dae
{
	class GameObject;

	class Scene
	{
	public:
		//explicit Scene(const std::string_view name);

		GameObject& CreateObject(const std::string_view name/*const std::unique_ptr<GameObject>& object*/);
		void TakeOwnership(std::unique_ptr<GameObject> pObject);
		std::unique_ptr<GameObject> ReleaseOwnership(GameObject& pObject);

		void LateInit();
		void Update();
		void FixedUpdate(float deltaT);
		void LateUpdate();
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		explicit Scene(const std::string_view name);
		//friend Scene& SceneManager::CreateScene(const std::string_view name);
		friend class SceneManager;

		std::string m_Name;
		std::vector<std::unique_ptr<GameObject>> m_Objects{};

		static unsigned int m_IdCounter;
	};

}
