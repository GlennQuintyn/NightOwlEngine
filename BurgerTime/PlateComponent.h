#pragma once
#include <BaseComponent.h>
#include <Observer.h>
#include <Subject.h>

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

		void SetIngredientFullCount(int count) { m_MaxIngredientsCount = count; };
		bool IsPlateFull() { return m_IngredientsCount >= m_MaxIngredientsCount; };

		void LateInit() override;
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override;

		Subject& GetSubject() { return m_Subject; }
		void Notify(GameObject* pObject, int event) override;

		PlateComponent(const PlateComponent& other) = delete;
		PlateComponent(PlateComponent&& other) = delete;
		PlateComponent& operator=(const PlateComponent& other) = delete;
		PlateComponent& operator=(PlateComponent&& other) = delete;

	private:
		Subject m_Subject;
		GameObject* m_pParentObject;
		RectColliderComponent* m_pCollidermpt;
		int m_IngredientsCount;
		int m_MaxIngredientsCount;
		int m_IntialColliderHeight;
	};
}