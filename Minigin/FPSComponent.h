#pragma once
#include "BaseComponent.h"
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;

	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent();
		~FPSComponent() = default;

		//todo: change fps mode to a seperate component and add precision of float to as option and maybe add code to make fps more or less volatile
		void SetFont(const std::shared_ptr<Font>& font) { m_Font = font; };
		void SetPosition(float x, float y) { m_Transform.SetPosition(x, y, 0.0f); };
		void SetTextColor(const SDL_Color& color) { m_TextColor = color; };
		void SetTextColor(unsigned char r, unsigned char g, unsigned char b) { m_TextColor = SDL_Color{ r, g, b }; };

		virtual void Update() override;
		virtual void Render() const override;

		//void SetTexture(const std::string& filename);
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;
	private:
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;
		SDL_Color m_TextColor;
		Transform m_Transform;
	};
}
