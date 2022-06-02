#pragma once
#include <BaseComponent.h>

namespace dae
{
	class GameObject;
	class Texture2D;

	class ButtonComponent final : public BaseComponent
	{
	public:
		ButtonComponent(GameObject* pParentObject);
		~ButtonComponent() = default;

		//make sure that component has a parent that isn't nullptr
		ButtonComponent(std::nullptr_t) = delete;

		void SetTextureNormal(const std::string& filename);
		void SetTextureSelected(const std::string& filename);
		void SetSelectedState(bool selected) { m_Selected = selected; };

		bool IsMouseInsideOfButton(const glm::ivec2& mousePos);

		void LateInit() override;
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override;
		void Reset() override {};

		ButtonComponent(const ButtonComponent& other) = delete;
		ButtonComponent(ButtonComponent&& other) = delete;
		ButtonComponent& operator=(const ButtonComponent& other) = delete;
		ButtonComponent& operator=(ButtonComponent&& other) = delete;

	private:
		GameObject* m_pParentObject;
		std::shared_ptr<Texture2D> m_pTextureNormal;
		std::shared_ptr<Texture2D> m_pTextureSelected;
		glm::ivec2 m_Size;
		bool m_Selected;
	};
}