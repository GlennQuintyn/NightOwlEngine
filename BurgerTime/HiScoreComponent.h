#pragma once
#include "BaseComponent.h"

namespace dae
{
	class GameObject;
	class TextComponent;
	class ScoreComponent;

	class HiScoreComponent final : public BaseComponent
	{
	public:
		HiScoreComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		HiScoreComponent(std::nullptr_t) = delete;

		~HiScoreComponent() = default;

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override {};

		void SetTextComponent(TextComponent& textComponent);
		void SetScoreComponent(ScoreComponent& scoreComponent);

		HiScoreComponent(const HiScoreComponent& other) = delete;
		HiScoreComponent(HiScoreComponent&& other) = delete;
		HiScoreComponent& operator=(const HiScoreComponent& other) = delete;
		HiScoreComponent& operator=(HiScoreComponent&& other) = delete;

	private:
		GameObject* m_pParentObject;
		TextComponent* m_pTextComponent;
		ScoreComponent* m_pScoreComponent;
		static int m_Score;
	};
}