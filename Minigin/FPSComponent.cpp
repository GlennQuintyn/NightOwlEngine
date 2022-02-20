#include "NightOwlEnginePCH.h"
#include "FPSComponent.h"
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Time.h"
#include <iostream>
#include <format>

dae::FPSComponent::FPSComponent()
	: m_Font{ nullptr }
	, m_TextTexture{ nullptr }
	, m_Transform{}
	, m_TextColor{ SDL_Color{255,255,255} }
{
}

void dae::FPSComponent::Update()
{
	if (!m_Font)
		return;

	const std::string& frameRateText = std::format("{:.2f} FPS", Time::GetInstance().GetFPS());

	//const SDL_Color color = { 255,255,255 }; // only white text is supported now
	const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), frameRateText.c_str(), m_TextColor);
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
}

void dae::FPSComponent::Render() const
{
	if (m_TextTexture != nullptr)
	{
		const auto& pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
	}
}
