#include "BurgerTimePCH.h"

//#include <stdio.h>
//#include <iostream>
////disabling all warnings that come from this math library
//#pragma warning(push)
//#pragma warning( disable : 26495 )
//#pragma warning( disable : 26812 )
//#pragma warning( disable : 26819 )
//#pragma warning( disable : 4201 )
//#include <glm/glm.hpp>
//#include <SDL.h>
//#pragma warning (pop)

//#include "Logger.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include "Enums.h"
#include "LivesComponent.h"

using namespace dae;

LivesComponent::LivesComponent(GameObject* pParentObject)
	: m_CurrentLives{ 3 }
	, m_IconSize{ -1.f,-1.f }
	, m_pParentObject{ nullptr }
	, m_pTexture{ nullptr }
{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("LIVESCOMPONENT:\tPARENT OBJECT WAS NOT GIVEN!");
}

void LivesComponent::SetTexture(const std::string& filename, float sizeX, float sizeY)
{
	if (sizeX > -FLT_EPSILON || sizeY > -FLT_EPSILON)
	{
		m_IconSize.x = sizeX;
		m_IconSize.y = sizeY;
	}
	//ResourceManager::GetInstance().Init
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::LivesComponent::Notify(GameObject*, int event)
{
	switch (Events(event))
	{
	case dae::Events::PeterPepper_Died:
		--m_CurrentLives;
		if (m_CurrentLives < 0)
			m_CurrentLives = 0;
		break;
	}

}

void LivesComponent::LateInit()
{
	if (!m_pTexture)
		Logger::GetInstance().LogError("LIVESCOMPONENT:\tTEXTURE WAS NOT GIVEN!");
}

void LivesComponent::Update()
{
	//m_pTextComponent->SetText("Lives: " + std::to_string(m_CurrentLives));
}

void LivesComponent::Render() const
{
	const auto& pos = m_pParentObject->GetWorldPosition();
	const glm::vec2 textureSize{ float(m_pTexture->GetWidth()) * m_IconSize.x,float(m_pTexture->GetHeight()) * m_IconSize.y };
	const float offset{ 5.f };
	float tempY{ pos.y };

	for (int count = 0; count < m_CurrentLives; count++)
	{
		if (m_IconSize.x < 0.f || m_IconSize.y < 0.f)
			Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, tempY);
		else
			Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, tempY, textureSize.x, textureSize.y);

		tempY -= offset + textureSize.y;
	}
}
