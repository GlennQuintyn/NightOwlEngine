#pragma once
#include "BaseComponent.h"
#include "Subject.h"

namespace dae
{
	class GameObject;
	//class Subject;

	class RectColiderComponent final : public BaseComponent
	{
	public:
		RectColiderComponent(GameObject* pParentObject);
		//make sure that component has a parent that isn't nullptr
		RectColiderComponent(std::nullptr_t) = delete;

		~RectColiderComponent() = default;

		virtual void LateInit() {};
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void Render() const;

		void SetDebugModeAndColor(bool drawDebug, SDL_Color debugColor = { 255, 255, 255, SDL_ALPHA_OPAQUE / 2 });
		Subject& GetSubject() { return m_Subject; }
		void UpdateOverlapping(RectColiderComponent* pOtherCollider);
		const SDL_Rect& GetRectangle() const { return m_ColliderBox; }

		RectColiderComponent(const RectColiderComponent& other) = delete;
		RectColiderComponent(RectColiderComponent&& other) = delete;
		RectColiderComponent& operator=(const RectColiderComponent& other) = delete;
		RectColiderComponent& operator=(RectColiderComponent&& other) = delete;

	private:
		SDL_Rect m_ColliderBox;
		Subject m_Subject;
		std::vector<RectColiderComponent*> m_pOtherOverlappingColliders;
		SDL_Color m_ColliderColor;
		GameObject* m_pParentObject;
		bool m_DebugRender;
	};
}

