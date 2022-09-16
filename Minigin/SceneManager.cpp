#include "NightOwlEnginePCH.h"
#include "SceneManager.h"

using namespace dae;

void SceneManager::LateInit()
{
	m_pScenes[m_ActiveSceneIndex]->LateInit();
}

void SceneManager::Update()
{
	m_pScenes[m_ActiveSceneIndex]->Update();
}

void SceneManager::FixedUpdate(float deltaT)
{
	m_pScenes[m_ActiveSceneIndex]->FixedUpdate(deltaT);
}

void SceneManager::LateUpdate()
{
	m_pScenes[m_ActiveSceneIndex]->LateUpdate();
}

void SceneManager::Render()
{
	m_pScenes[m_ActiveSceneIndex]->Render();
}

void dae::SceneManager::ResetActiveScene()
{
	m_pScenes[m_ActiveSceneIndex]->Reset();
}

void dae::SceneManager::SetActiveScene(int index)
{
	if (index < 0 || index > static_cast<int>(m_pScenes.size()) - 1)
		return;

	m_ActiveSceneIndex = index;
}

void dae::SceneManager::GotoNextScene()
{
	++m_ActiveSceneIndex;

	if (m_ActiveSceneIndex >= static_cast<int>(m_pScenes.size()))
		m_ActiveSceneIndex = 0;
}

void dae::SceneManager::GotoPreviousScene()
{
	--m_ActiveSceneIndex;

	if (m_ActiveSceneIndex < 0)
		m_ActiveSceneIndex = static_cast<int>(m_pScenes.size()) - 1;
}

Scene& SceneManager::CreateScene(const std::string_view name)
{
	m_pScenes.push_back(std::unique_ptr<Scene>(new Scene{ name }));
	return *m_pScenes.back();
}
