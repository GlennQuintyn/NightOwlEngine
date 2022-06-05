#include "NightOwlEnginePCH.h"
#include "ColliderManager.h"
#include "SceneManager.h"
#include "GameObject.h"

void dae::ColliderManager::RegisterRectCollider(RectColliderComponent* pCollider)
{
	m_pRectColliderComponents.emplace_back(pCollider);
}

void dae::ColliderManager::Update()
{
	if (m_pRectColliderComponents.empty())
		return;

	int activeSceneIndex = SceneManager::GetInstance().GetActiveScene();

	for (size_t index1 = 0; index1 < m_pRectColliderComponents.size() - 1; index1++)
	{
		//if the index of the first collider is not from the active scene or it's not active, then don't update it.
		if (m_pRectColliderComponents[index1]->GetSceneId() != activeSceneIndex || !m_pRectColliderComponents[index1]->IsEnabled())
			continue;

		for (size_t index2 = index1 + 1; index2 < m_pRectColliderComponents.size(); index2++)
		{
			//if the index of the 2nd collider is not from the active scene or it's not active, then don't update it.
			if (m_pRectColliderComponents[index2]->GetSceneId() != activeSceneIndex || !m_pRectColliderComponents[index2]->IsEnabled())
				continue;

			//check if they belong to the same scene
			if (m_pRectColliderComponents[index1]->GetSceneId() == m_pRectColliderComponents[index2]->GetSceneId())
			{
				m_pRectColliderComponents[index1]->UpdateOverlapping(m_pRectColliderComponents[index2]);
			}
		}
	}
}
