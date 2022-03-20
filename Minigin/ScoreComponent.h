#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class GameObject;
	class TextComponent;

	class ScoreComponent :public BaseComponent, public Observer
	{
	public:
		ScoreComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		ScoreComponent(std::nullptr_t) = delete;

		~ScoreComponent() = default;

		void SetTextComponent(TextComponent& pTextComponent);

		void Notify(GameObject* pObject, int event) override;

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};

	private:
		int m_CurrentScore;

		GameObject* m_pParentObject;
		TextComponent* m_pTextComponent;
	};
}