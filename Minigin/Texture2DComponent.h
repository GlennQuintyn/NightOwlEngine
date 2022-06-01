#pragma once
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;
	class GameObject;

	class Texture2DComponent final : public BaseComponent
	{
	public:
		Texture2DComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		Texture2DComponent(std::nullptr_t) = delete;

		~Texture2DComponent() = default;

		void LateInit() override;
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override;
		void Reset() override {};

		void SetTexture(const std::string& filename, float width = -1.f, float height = -1.f);
		const glm::vec2& GetTextureSize() const { return m_Size; };

		Texture2DComponent(const Texture2DComponent& other) = delete;
		Texture2DComponent(Texture2DComponent&& other) = delete;
		Texture2DComponent& operator=(const Texture2DComponent& other) = delete;
		Texture2DComponent& operator=(Texture2DComponent&& other) = delete;
	private:
		std::shared_ptr<Texture2D> m_pTexture{};
		glm::vec2 m_Size;
		GameObject* m_pParentObject;
	};
}
