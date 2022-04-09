#include "NightOwlEnginePCH.h"
#include "Texture2D.h"

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

dae::Texture2D::Texture2D(SDL_Texture* pTexture)
	: m_pTexture{ nullptr }
	, m_Width{ 0 }
	, m_Height{ 0 }
{
	m_pTexture = pTexture;

	if (m_pTexture)
		SDL_QueryTexture(m_pTexture, nullptr, nullptr, &m_Width, &m_Height);
	else
		Logger::GetInstance().LogError("TEXTURE2D:\tNO TEXTURE WAS GIVEN!");
}
