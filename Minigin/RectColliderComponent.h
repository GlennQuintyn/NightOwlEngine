#pragma once
#include "BaseComponent.h"
#include "Subject.h"
//#include "Observer.h"

namespace dae
{
	class GameObject;

	class RectColliderComponent final : public BaseComponent//, public Observer
	{
	public:
		RectColliderComponent(GameObject* pParentObject);
		//make sure that component has a parent that isn't nullptr
		RectColliderComponent(std::nullptr_t) = delete;

		~RectColliderComponent() = default;

		virtual void LateInit() {};
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void Render() const;

		void Init(SDL_Rect colliderBox, int id, bool debugRender = false, SDL_Color debugColor = { 255, 255, 255, SDL_ALPHA_OPAQUE / 2 });

		void SetDebugMode(bool drawDebug) { m_DebugRender = drawDebug; }
		void SetDebugColliderColor(SDL_Color debugColor) { m_ColliderColor = debugColor; }

		//int GetID() const { return m_IdNr; };
		//void Notify(GameObject*, int) override {};

		Subject& GetSubject() { return m_Subject; }
		void UpdateOverlapping(RectColliderComponent* pOtherCollider);
		const SDL_Rect& GetRectangle() const { return m_ColliderBox; }
		void SetRectangle(SDL_Rect collider) { m_ColliderBox = collider; }

		RectColliderComponent(const RectColliderComponent& other) = delete;
		RectColliderComponent(RectColliderComponent&& other) = delete;
		RectColliderComponent& operator=(const RectColliderComponent& other) = delete;
		RectColliderComponent& operator=(RectColliderComponent&& other) = delete;

	private:
		SDL_Rect m_ColliderBox;
		Subject m_Subject;
		std::vector<RectColliderComponent*> m_pOtherOverlappingColliders;
		SDL_Color m_ColliderColor;
		GameObject* m_pParentObject;
		int m_IdNr;//TODO: remove the ID
		bool m_DebugRender;
	};
}

