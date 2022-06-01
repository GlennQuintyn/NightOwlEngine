#pragma once
#include "BaseComponent.h"

namespace dae
{
	class GameObject;
	class TextComponent;

	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		FPSComponent(std::nullptr_t) = delete;

		~FPSComponent() = default;

		void SetTextComponent(TextComponent& pTextComponent);

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override {};

		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;
	private:
		GameObject* m_pParentObject;
		TextComponent* m_pTextComponent;
	};
}
