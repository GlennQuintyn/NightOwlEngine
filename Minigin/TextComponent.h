#pragma once
#include "BaseComponent.h"


namespace dae
{
	class TextComponent : public BaseComponent
	{
	public:
		TextComponent() = default;
		~TextComponent() = default;

		virtual void Update() override {};
		virtual void Render() const override {};

		//void SetTexture(const std::string& filename);

	private:
		//std::shared_ptr<Texture2D> m_Texture{};
	};
}


//	class Texture2D;
//
//	class Texture2DComponent final : public BaseComponent
//	{
//	public:
//		Texture2DComponent();
//		~Texture2DComponent();
//
//		virtual void Update() override {};
//		virtual void Render() const override {};
//
//		void SetTexture(const std::string& filename);
//
//	private:
//		std::shared_ptr<Texture2D> m_Texture{};
//	};
//}
