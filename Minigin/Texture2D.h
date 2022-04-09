#pragma once
struct SDL_Texture;
namespace dae
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D
	{
	public:
		explicit Texture2D(SDL_Texture* pTexture);
		~Texture2D();

		int GetWidth() const { return m_Width; };
		int GetHeight() const { return m_Height; };
		
		SDL_Texture* GetSDLTexture() const;

		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&&) = delete;
	private:
		SDL_Texture* m_pTexture;
		int m_Width, m_Height;
	};
}
