#include "NightOwlEnginePCH.h"
#include "RectColiderComponent.h"
#include "ColliderManager.h"
#include "Renderer.h"

dae::RectColiderComponent::RectColiderComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_Subject{}
	, m_ColliderBox{}
	, m_ColliderColor{ 255, 255, 255, SDL_ALPHA_OPAQUE / 2 }
	, m_DebugRender{ false }
{
	ColliderManager::GetInstance().RegisterRectCollider(this);
}

void dae::RectColiderComponent::Render() const
{
	if (!m_DebugRender)
		return;

	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), m_ColliderColor.r, m_ColliderColor.g, m_ColliderColor.b, m_ColliderColor.a);
	SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &m_ColliderBox);
}

void dae::RectColiderComponent::SetDebugModeAndColor(bool drawDebug, SDL_Color debugColor)
{
	m_DebugRender = drawDebug;
	m_ColliderColor = debugColor;
}

void dae::RectColiderComponent::UpdateOverlapping(RectColiderComponent* pOtherCollider)
{
	SDL_Rect r1{ m_ColliderBox };
	SDL_Rect r2{ pOtherCollider->m_ColliderBox };

	auto it = std::find(m_pOtherOverlappingColliders.begin(), m_pOtherOverlappingColliders.end(), pOtherCollider);
	//this code is fromt the prog2 framework :)

	// If one rectangle is on left side of the other one or
	// If the rectangle is under the other one
	if ((r1.x + r1.w) < r2.x || (r2.x + r2.w) < r1.x ||
		r1.y > (r2.y + r2.h) || r2.y > (r1.y + r1.h))
	{
		//if collider was found, that means there were colliding but not anymore
		if (it != m_pOtherOverlappingColliders.end())
		{
			m_Subject.Notify(pOtherCollider->m_pParentObject, 1);
			pOtherCollider->m_Subject.Notify(m_pParentObject, 1);
		}

		m_pOtherOverlappingColliders.erase(it);

		return;
	}

	//if they collide and the other collider is already in the vector then leave it be
	if (it != m_pOtherOverlappingColliders.end())
		return;

	m_Subject.Notify(pOtherCollider->m_pParentObject, 0);
	pOtherCollider->m_Subject.Notify(m_pParentObject, 0);

	m_pOtherOverlappingColliders.emplace_back(pOtherCollider);
}
