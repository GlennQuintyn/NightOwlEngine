#pragma once
#include <BaseComponent.h>
#include <Observer.h>

namespace dae
{
	class GameObject;

	class PepperComponent final : public BaseComponent, public Observer
	{
	public:
		PepperComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		PepperComponent(std::nullptr_t) = delete;

		~PepperComponent() = default;

		void  SetSpriteDuration(const float duration) { m_SpriteDuration = duration; };
		void SetResetPos(const float xPos, const float yPos) { m_ResetPos = { xPos,yPos }; };

		const float GetPepperedDuration() const { return m_PepperedDuration; };
		void Reset();
		void Thrown();

		void Notify(GameObject* pObject, int event) override;

		void LateInit() override {};
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};

		PepperComponent(const PepperComponent& other) = delete;
		PepperComponent(PepperComponent&& other) = delete;
		PepperComponent& operator=(const PepperComponent& other) = delete;
		PepperComponent& operator=(PepperComponent&& other) = delete;

	private:
		GameObject* m_pParentObject;
		glm::vec2 m_ResetPos;
		float m_PepperedDuration;
		float m_SpriteDuration;
		float m_TimeLeft;
		bool m_hasBeenThrown;
	};
}