#include "NightOwlEnginePCH.h"
#include "TextComponent.h"
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(GameObject* pParentObject)
	: m_Text{ "Lorem Ipsum" }
	, m_Font{ nullptr }
	, m_TextTexture{ nullptr }
	, m_Transform{}
	, m_NeedsUpdate{ true }
	, m_FpsCounterMode{}
	, m_TextColor{ SDL_Color{255,255,255} }
	, m_pParentObject{nullptr}
{
	if (pParentObject)
	{
		m_pParentObject = pParentObject;
	}
}

void dae::TextComponent::SetText(const std::string_view text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetTextColor(const SDL_Color& color)
{
	m_TextColor = color;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetTextColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_TextColor = SDL_Color{ r, g, b };
	m_NeedsUpdate = true;
}

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		if (!m_Font)
			return;

		//const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_TextColor);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_TextTexture != nullptr)
	{
		const auto& pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
	}
}
