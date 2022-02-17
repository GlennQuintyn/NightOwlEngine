#include "NightOwlEnginePCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaT)
{
	//UNREFERENCED_PARAMETER(deltaT);
	for (auto& scene : m_pScenes)
	{
		scene->Update(deltaT);
	}
}

void dae::SceneManager::FixedUpdate(float deltaT)
{
	//UNREFERENCED_PARAMETER(deltaT);
	for (const auto& scene : m_pScenes)
	{
		scene->FixedUpdate(deltaT);
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_pScenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_pScenes.push_back(scene);
	return *scene;
}
