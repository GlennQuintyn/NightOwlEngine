#pragma once
#include <BaseComponent.h>

namespace dae
{
	class GameObject;

	class LadderComponent final : public BaseComponent
	{
	public:
		LadderComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		LadderComponent(std::nullptr_t) = delete;

		~LadderComponent() = default;

		void LateInit() override {};
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override {};

		LadderComponent(const LadderComponent& other) = delete;
		LadderComponent(LadderComponent&& other) = delete;
		LadderComponent& operator=(const LadderComponent& other) = delete;
		LadderComponent& operator=(LadderComponent&& other) = delete;

	private:
		GameObject* m_pParentObject;
	};
}