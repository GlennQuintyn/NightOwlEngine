#pragma once
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

	class Texture2DComponent final : public BaseComponent
	{
	public:
		Texture2DComponent();
		~Texture2DComponent();
		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_Texture{};
	};
}
