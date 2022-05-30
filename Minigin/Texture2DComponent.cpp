#include "NightOwlEnginePCH.h"
#include "Texture2DComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

using namespace dae;

Texture2DComponent::Texture2DComponent(GameObject* pParentObject)
	: m_pParentObject{ nullptr }
	, m_Size{ -1.f, -1.f }
{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("TEXTURE2DCOMPONENT:\tPARENT OBJECT WAS NOT GIVEN!");
}

void Texture2DComponent::LateInit()
{
	if (!m_pTexture)
		Logger::GetInstance().LogError("TEXTURE2DCOMPONENT:\tTEXTURE WAS NOT GIVEN!");
}

void Texture2DComponent::Render() const
{
	if (m_pTexture != nullptr)
	{
		const auto& pos = m_pParentObject->GetWorldPosition();
		if (m_Size.x < 0.f || m_Size.y < 0.f)
			Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
		else
			Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, m_Size.x, m_Size.y);
	}
}

void Texture2DComponent::SetTexture(const std::string& filename, float width, float height)
{
	m_Size.x = width;
	m_Size.y = height;
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}
