#pragma once
#include <BaseComponent.h>
#include <Observer.h>

namespace dae
{
	class GameObject;
	class RectColliderComponent;

	class PlateComponent final : public BaseComponent, public Observer
	{
	public:
		PlateComponent(GameObject* pParentObject);
		//make sure that component has a parent that isn't nullptr
		PlateComponent(std::nullptr_t) = delete;

		~PlateComponent() = default;

		void LateInit() override;
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override {};

		//Subject& GetSubject() { return m_Subject; }
		void Notify(GameObject* pObject, int event) override;

		PlateComponent(const PlateComponent& other) = delete;
		PlateComponent(PlateComponent&& other) = delete;
		PlateComponent& operator=(const PlateComponent& other) = delete;
		PlateComponent& operator=(PlateComponent&& other) = delete;

	private:
		GameObject* m_pParentObject;
		RectColliderComponent* m_pCollidermpt;
	};
}


