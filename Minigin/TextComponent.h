#pragma once
#include "BaseComponent.h"
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent final : public BaseComponent
	{
	public:
		TextComponent();
		~TextComponent() = default;

		void SetFont(const std::shared_ptr<Font>& font) { m_Font = font; };
		void SetText(const std::string_view& text);
		void SetPosition(float x, float y) { m_Transform.SetPosition(x, y, 0.0f); };
		void SetFpsMode(bool state) { m_FpsCounterMode = state; };
		void SetTextColor(const SDL_Color& color);
		void SetTextColor(unsigned char r, unsigned char g, unsigned char b);

		virtual void Update() override;
		virtual void Render() const override;

		//void SetTexture(const std::string& filename);
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		//std::shared_ptr<Texture2D> m_Texture{};
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;
		SDL_Color m_TextColor;
		Transform m_Transform;
		bool m_FpsCounterMode;
		bool m_NeedsUpdate;
	};
}