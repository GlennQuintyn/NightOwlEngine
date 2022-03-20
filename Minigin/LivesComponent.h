#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class GameObject;
	class TextComponent;

	class LivesComponent final : public BaseComponent, public Observer
	{
	public:
		LivesComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		LivesComponent(std::nullptr_t) = delete;

		~LivesComponent() = default;

		void SetTextComponent(TextComponent& pTextComponent);

		void Notify(GameObject* pObject, int event) override;

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};

	private:
		int m_CurrentLives;

		GameObject* m_pParentObject;
		TextComponent* m_pTextComponent;
	};
}
