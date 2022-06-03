#pragma once
#include <BaseComponent.h>
#include <array>
#include "RectColliderComponent.h"
#include "Enums.h"

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

		bool CanGoLeft()const { return m_CanGoLeft && m_Enabled; };
		bool CanGoRight()const { return m_CanGoRight && m_Enabled; };
		bool CanGoUp()const { return m_CanGoUp && m_Enabled; };
		bool CanGoDown()const { return m_CanGoDown && m_Enabled; };

		GameObject* GetTouchingPlatformLeft();
		GameObject* GetTouchingPlatformRight();
		GameObject* GetTouchingLadderUp();
		GameObject* GetTouchingLadderDown();

		void SetEnabled(bool enabled) { m_Enabled = enabled; };
		WalkingDirection GetLastWalkingDirection() const { return m_LastDirection; };

		void LateInit() override {};
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override {};

	private:
		GameObject* m_pParentObject;

		class HitBoxObserver;
		std::unique_ptr<std::array<HitBoxObserver, 4>> m_pImpl;

		WalkingDirection m_LastDirection;
		bool m_Enabled;

		bool m_CanGoLeft;
		bool m_CanGoRight;
		bool m_CanGoUp;
		bool m_CanGoDown;
	};
}
