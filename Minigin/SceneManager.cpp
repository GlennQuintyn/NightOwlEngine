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
	m_pScenes[m_ActiveSceneIndex]->Update();

	//for (const auto& scene : m_pScenes)
	//{
	//	scene->Update();
	//}
}

void SceneManager::FixedUpdate(float deltaT)
{
	m_pScenes[m_ActiveSceneIndex]->FixedUpdate(deltaT);

	//for (const auto& scene : m_pScenes)
	//{
	//	scene->FixedUpdate(deltaT);
	//}
}

void SceneManager::LateUpdate()
{
	m_pScenes[m_ActiveSceneIndex]->LateUpdate();

	//for (const auto& scene : m_pScenes)
	//{
	//	scene->LateUpdate();
	//}
}

void SceneManager::Render()
{
	m_pScenes[m_ActiveSceneIndex]->Render();

	//for (const auto& scene : m_pScenes)
	//{
	//	scene->Render();
	//}
}

void dae::SceneManager::ResetActiveScene()
{
	m_pScenes[m_ActiveSceneIndex]->Reset();
}

void dae::SceneManager::SetActiveScene(size_t index)
{
	if (index<0 || index>m_pScenes.size() - 1)
		return;

	m_ActiveSceneIndex = index;
}

void dae::SceneManager::GotoNextScene()
{
	++m_ActiveSceneIndex;

	if (m_ActiveSceneIndex + 1 > m_pScenes.size())
		m_ActiveSceneIndex = 0;
}

void dae::SceneManager::GotoPreviousScene()
{
	--m_ActiveSceneIndex;

	if (m_ActiveSceneIndex < 0)
		m_ActiveSceneIndex = m_pScenes.size() - 1;
}

Scene& SceneManager::CreateScene(const std::string_view name)
{
	m_pScenes.push_back(std::unique_ptr<Scene>(new Scene{ name }));
	return *m_pScenes.back();
}
