#pragma once
#include <BaseComponent.h>

namespace dae
{
	class GameObject;

	class WalkingPlatformComponent final : public BaseComponent
	{
	public:
		WalkingPlatformComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		WalkingPlatformComponent(std::nullptr_t) = delete;

		~WalkingPlatformComponent() = default;

		void LateInit() override {};
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override {};

		WalkingPlatformComponent(const WalkingPlatformComponent & other) = delete;
		WalkingPlatformComponent(WalkingPlatformComponent && other) = delete;
		WalkingPlatformComponent& operator=(const WalkingPlatformComponent & other) = delete;
		WalkingPlatformComponent& operator=(WalkingPlatformComponent && other) = delete;

	private:
		GameObject* m_pParentObject;
	};
}


