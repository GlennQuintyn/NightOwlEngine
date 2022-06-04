#pragma once
#include <BaseComponent.h>
#include <Observer.h>
#include <Subject.h>
#include <array>

namespace dae
{
	class GameObject;
	class RectColliderComponent;

	class IngredientComponent final : public BaseComponent, public Observer
	{
	public:
		IngredientComponent(GameObject* pParentObject);
		//make sure that component has a parent that isn't nullptr
		IngredientComponent(std::nullptr_t) = delete;

		~IngredientComponent() = default;

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override;

		void Notify(GameObject* pObject, int event) override;
		Subject& GetSubject() { return m_Subject; }

		void SetSpawnLocation(float x, float y);

		bool IsFalling() const;

		IngredientComponent(const IngredientComponent& other) = delete;
		IngredientComponent(IngredientComponent&& other) = delete;
		IngredientComponent& operator=(const IngredientComponent& other) = delete;
		IngredientComponent& operator=(IngredientComponent&& other) = delete;

	private:
		enum class IngridientState
		{
			Falling,
			OnPlatform,
			OnPlate,
			//is to stop the ingredients from sticking together when needing to fall an extra level
			Wait
		};

		Subject m_Subject;
		glm::vec2 m_SpawnPos;
		GameObject* m_pParentObject;
		GameObject* m_pPlatformObject;
		RectColliderComponent* m_ColliderLeft;
		RectColliderComponent* m_ColliderCenter;
		RectColliderComponent* m_ColliderRight;

		const float m_FallSpeed;
		IngridientState m_State;
		int m_EnemyOnIngredientCount;

		class HitBoxObserver;
		std::unique_ptr<std::array<HitBoxObserver, 3>> m_pImpl;

		bool m_ColliderLeftHit;
		bool m_ColliderCenterHit;
		bool m_ColliderRightHit;

		bool m_FallExtraLevel;
		bool m_HasSettled;
	};
}