#pragma once
#include "Singleton.h"

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_clearColor{};

	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		//render texture at source size
		void RenderTexture(const Texture2D& texture, float x, float y) const;
		//render texture with a size specified
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		//render texture with a size source rectangle specified
		void RenderTexture(const Texture2D& texture, const float srcX, const float srcY, const float srcWidth, const float srcHeight,
			const float dstX, const float dstY, const float dstWidth, const float dstHeight) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

