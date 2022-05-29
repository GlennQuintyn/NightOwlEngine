#pragma once
#include <BaseComponent.h>
#include <array>

namespace dae
{
	class GameObject;

	class MovementComponent final : public BaseComponent
	{
	public:
		MovementComponent(GameObject* pParentObject);
		~MovementComponent();

		//make sure that component has a parent that isn't nullptr
		MovementComponent(std::nullptr_t) = delete;

		bool CanGoLeft()const { return m_CanGoLeft; };
		bool CanGoRight()const { return m_CanGoRight; };
		bool CanGoUp()const { return m_CanGoUp; };
		bool CanGoDown()const { return m_CanGoDown; };

		void LateInit() override {};
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override {};

	private:
		GameObject* m_pParentObject;

		class HitBoxObserver;
		std::unique_ptr<std::array<HitBoxObserver, 4>> m_pImpl;

		bool m_CanGoLeft;
		bool m_CanGoRight;
		bool m_CanGoUp;
		bool m_CanGoDown;
	};
}
