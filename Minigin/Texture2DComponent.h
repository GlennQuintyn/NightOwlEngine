#pragma once
#include "BaseComponent.h"
#include "Transform.h"

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

		virtual void Update() override;
		virtual void Render() const override;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y) { m_Transform.SetPosition(x, y, 0.0f); };

		Texture2DComponent(const Texture2DComponent& other) = delete;
		Texture2DComponent(Texture2DComponent&& other) = delete;
		Texture2DComponent& operator=(const Texture2DComponent& other) = delete;
		Texture2DComponent& operator=(Texture2DComponent&& other) = delete;
	private:
		std::shared_ptr<Texture2D> m_Texture{};
		Transform m_Transform;
		GameObject* m_pParentObject;
	};
}
