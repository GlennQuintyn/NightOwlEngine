#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "Subject.h"

namespace dae
{
	class RectColliderComponent final : public BaseComponent//, public Observer
	{
	public:
		RectColliderComponent(GameObject* pParentObject);
		//make sure that component has a parent that isn't nullptr
		RectColliderComponent(std::nullptr_t) = delete;

		~RectColliderComponent() = default;

		void LateInit() override;
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override;
		void Reset() override {};

		const int GetSceneId() const { return m_SceneIdNr; };
		void SetSceneId(int id) { m_SceneIdNr = id; };

		void Init(SDL_Rect colliderBox, int id, bool debugRender = false, SDL_Color debugColor = { 255, 255, 255, SDL_ALPHA_OPAQUE / 2 });

		void SetDebugMode(bool drawDebug) { m_DebugRender = drawDebug; }
		void SetDebugColliderColor(SDL_Color debugColor) { m_ColliderColor = debugColor; }

		const bool IsEnabled() const { return m_pParentObject->IsEnabled(); };

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
		int m_SceneIdNr;
		bool m_DebugRender;
		bool m_Ready;
	};
}

