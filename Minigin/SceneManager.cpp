#include "NightOwlEnginePCH.h"
#include "SceneManager.h"

using namespace dae;

void SceneManager::LateInit()
{
	for (const auto& scene : m_pScenes)
	{
		scene->LateInit();
	}
}

void SceneManager::Update()
{
	for (const auto& scene : m_pScenes)
	{
		scene->Update();
	}
}

void SceneManager::FixedUpdate(float deltaT)
{
	for (const auto& scene : m_pScenes)
	{
		scene->FixedUpdate(deltaT);
	}
}

void SceneManager::LateUpdate()
{
	for (const auto& scene : m_pScenes)
	{
		scene->LateUpdate();
	}
}

void SceneManager::Render()
{
	for (const auto& scene : m_pScenes)
	{
		scene->Render();
	}
}

Scene& SceneManager::CreateScene(const std::string_view name)
{
	m_pScenes.push_back(std::unique_ptr<Scene>(new Scene{ name }));
	return *m_pScenes.back();
}
