#pragma once
#include <BaseComponent.h>
#include <Observer.h>
#include <Subject.h>

namespace dae
{
	class GameObject;
	class TextComponent;

	class ScoreComponent :public BaseComponent, public Observer
	{
	public:
		ScoreComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		ScoreComponent(std::nullptr_t) = delete;

		~ScoreComponent() = default;

		void SetTextComponent(TextComponent& pTextComponent);
		const int GetCurrentScore() const { return m_CurrentScore; };

		void Notify(GameObject* pObject, int event) override;

		Subject& GetSubject() { return m_Subject; }

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override { m_CurrentScore = 0; };

		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;
	private:
		Subject m_Subject;
		GameObject* m_pParentObject;
		TextComponent* m_pTextComponent;
		int m_CurrentScore;
		int m_ExtraLifeTreshold;
	};
}