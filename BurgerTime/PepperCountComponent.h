#pragma once
#include "BaseComponent.h"

namespace dae
{
	class GameObject;
	class TextComponent;

	class PepperCountComponent final : public BaseComponent
	{

	public:
		PepperCountComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		PepperCountComponent(std::nullptr_t) = delete;

		~PepperCountComponent() = default;

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override {};

		void SetTextComponent(TextComponent& pTextComponent);

		PepperCountComponent(const PepperCountComponent& other) = delete;
		PepperCountComponent(PepperCountComponent&& other) = delete;
		PepperCountComponent& operator=(const PepperCountComponent& other) = delete;
		PepperCountComponent& operator=(PepperCountComponent&& other) = delete;

	private:
		GameObject* m_pParentObject;
		TextComponent* m_pTextComponent;
		int m_PepperCount;

	};
}