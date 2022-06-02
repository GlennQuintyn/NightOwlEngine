#include "BurgerTimePCH.h"
#include "ButtonComponent.h"
#include <GameObject.h>
#include <ResourceManager.h>
#include <Renderer.h>
#include <Texture2D.h>

dae::ButtonComponent::ButtonComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_pTextureNormal{}
	, m_pTextureSelected{}
	, m_Size{}
	, m_Selected{ false }
{
}

void dae::ButtonComponent::SetTextureNormal(const std::string& filename)
{
	m_pTextureNormal = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::ButtonComponent::SetTextureSelected(const std::string& filename)
{
	m_pTextureSelected = ResourceManager::GetInstance().LoadTexture(filename);
}

bool dae::ButtonComponent::IsMouseInsideOfButton(const glm::ivec2& mousePos)
{
	const auto& pos = m_pParentObject->GetWorldPosition();

	if ((mousePos.x >= pos.x && mousePos.x <= pos.x + m_Size.x) &&
		(mousePos.y >= pos.y && mousePos.y <= pos.y + m_Size.y))
	{
		return true;
	}
	return false;
}

void dae::ButtonComponent::LateInit()
{
	if (!m_pTextureNormal)
		Logger::GetInstance().LogError("BUTTONCOMPONENT:\tTEXTURE NOT SELECTED WAS NOT GIVEN!");
	else
	{
		m_Size.x = m_pTextureNormal->GetWidth();
		m_Size.y = m_pTextureNormal->GetHeight();
	}
	if (!m_pTextureSelected)
		Logger::GetInstance().LogError("BUTTONCOMPONENT:\tTEXTURE SELECTED WAS NOT GIVEN!");
}

void dae::ButtonComponent::Render() const
{
	const auto& pos = m_pParentObject->GetWorldPosition();

	if (!m_Selected && m_pTextureSelected)
		Renderer::GetInstance().RenderTexture(*m_pTextureNormal, pos.x, pos.y);
	else if (m_Selected && m_pTextureNormal)
		Renderer::GetInstance().RenderTexture(*m_pTextureSelected, pos.x, pos.y);
}
