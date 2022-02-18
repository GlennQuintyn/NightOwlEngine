#pragma once
#include "BaseComponent.h"
#include "Transform.h"

namespace dae
{
	class Texture2D;

	class Texture2DComponent final : public BaseComponent
	{
	public:
		Texture2DComponent() = default;
		~Texture2DComponent() = default;

		virtual void Update() override;
		virtual void Render() const override;

		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_Texture{};
		Transform m_Transform;
	};
}
