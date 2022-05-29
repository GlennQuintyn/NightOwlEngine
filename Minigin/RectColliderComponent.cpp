#include "NightOwlEnginePCH.h"
#include "RectColliderComponent.h"
#include "ColliderManager.h"
#include "Renderer.h"
#include "GameObject.h"

dae::RectColliderComponent::RectColliderComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_Subject{}
	, m_ColliderBox{}
	, m_ColliderColor{ 255, 255, 255, SDL_ALPHA_OPAQUE / 2 }
	, m_IdNr{ 0 }
	, m_DebugRender{ false }
{
	ColliderManager::GetInstance().RegisterRectCollider(this);
}

void dae::RectColliderComponent::Render() const
{
	if (!m_DebugRender)
		return;

	auto renderer = Renderer::GetInstance().GetSDLRenderer();

	SDL_SetRenderDrawColor(renderer, m_ColliderColor.r, m_ColliderColor.g, m_ColliderColor.b, m_ColliderColor.a);

	auto& pos = m_pParentObject->GetWorldPosition();
	float xPos{ pos.x + m_ColliderBox.x };
	float yPos{ pos.y + m_ColliderBox.y };

	//bottom line
	SDL_RenderDrawLineF(renderer, xPos, yPos, xPos + m_ColliderBox.w, yPos);
	//right line
	SDL_RenderDrawLineF(renderer, xPos + m_ColliderBox.w, yPos, xPos + m_ColliderBox.w, yPos + m_ColliderBox.h);
	//top line
	SDL_RenderDrawLineF(renderer, xPos, yPos + m_ColliderBox.h, xPos + m_ColliderBox.w, yPos + m_ColliderBox.h);
	//left line
	SDL_RenderDrawLineF(renderer, xPos, yPos, xPos, yPos + m_ColliderBox.h);


	//SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &m_ColliderBox);
}

void dae::RectColliderComponent::Init(SDL_Rect colliderBox, int id, bool debugRender, SDL_Color debugColor)
{
	m_ColliderBox = colliderBox;
	m_IdNr = id;
	m_DebugRender = debugRender;
	m_ColliderColor = debugColor;
}

void dae::RectColliderComponent::UpdateOverlapping(RectColliderComponent* pOtherCollider)
{
	SDL_Rect r1{ m_ColliderBox };//should get world position
	SDL_Rect r2{ pOtherCollider->m_ColliderBox };

	auto& position = m_pParentObject->GetWorldPosition();
	auto& positionOther = pOtherCollider->m_pParentObject->GetWorldPosition();

	r1.x += static_cast<int>(position.x);
	r1.y += static_cast<int>(position.y);
	r2.x += static_cast<int>(positionOther.x);
	r2.y += static_cast<int>(positionOther.y);

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
			m_pOtherOverlappingColliders.erase(it);
		}

		return;
	}

	//if they collide and the other collider is already in the vector then leave it be
	if (it != m_pOtherOverlappingColliders.end())
		return;

	m_Subject.Notify(pOtherCollider->m_pParentObject, 0);
	pOtherCollider->m_Subject.Notify(m_pParentObject, 0);

	m_pOtherOverlappingColliders.emplace_back(pOtherCollider);
}
