#include "NightOwlEnginePCH.h"
#include "ColliderManager.h"

void dae::ColliderManager::RegisterRectCollider(RectColiderComponent* pCollider)
{
	m_pRectColliderComponents.emplace_back(pCollider);
}

void dae::ColliderManager::Update()
{
	for (size_t index1 = 0; index1 < m_pRectColliderComponents.size() - 1; index1++)
	{
		for (size_t index2 = index1 + 1; index2 < m_pRectColliderComponents.size(); index2++)
		{
			m_pRectColliderComponents[index1]->UpdateOverlapping(m_pRectColliderComponents[index2]);

			//if (IsOverlapping(m_pRectColliderComponents[index1]->GetRectangle(), m_pRectColliderComponents[index2]->GetRectangle()))
			//{
			//	//m_pRectColliderComponents[index1]->GetSubject().Notify(nullptr, 0);
			//	//m_pRectColliderComponents[index2]->GetSubject().Notify(nullptr, 0);
			//
			//	//m_pRectColliderPairs.emplace_back(m_pRectColliderComponents[index1], m_pRectColliderComponents[index2]);
			//}
			//else
			//{
			//	auto it = std::find(m_pRectColliderPairs.begin(), m_pRectColliderPairs.end(), std::make_pair(m_pRectColliderComponents[index1], m_pRectColliderComponents[index2]));
			//	if (it != m_pRectColliderPairs.end())
			//	{
			//		it->first->GetSubject().Notify(nullptr, 1);
			//		it->second->GetSubject().Notify(nullptr, 1);
			//	}
			//	//for (auto& pCollider : m_pRectColliderPairs)
			//	//{
			//	//	if (pCollider.first == m_pRectColliderComponents[index1] &&
			//	//		pCollider.second == m_pRectColliderComponents[index2])
			//	//	{
			//	//	}
			//	//}
			//}
		}
	}
}
