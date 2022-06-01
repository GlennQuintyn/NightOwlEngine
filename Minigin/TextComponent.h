#pragma once
#include "BaseComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class GameObject;

	class TextComponent final : public BaseComponent
	{
	public:
		TextComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		TextComponent(std::nullptr_t) = delete;

		~TextComponent() = default;

		void SetFont(const std::shared_ptr<Font>& font) { m_Font = font; };
		void SetText(const std::string_view text);
		void SetTextColor(const SDL_Color& color);
		void SetTextColor(unsigned char r, unsigned char g, unsigned char b);

		void Setup(const std::shared_ptr<Font>& font, const SDL_Color& color = SDL_Color{ 255, 255, 255 }, const std::string_view text = "0");

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override;
		void Reset() override {};

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;

		SDL_Color m_TextColor;
		GameObject* m_pParentObject;

		bool m_NeedsUpdate;
	};
}