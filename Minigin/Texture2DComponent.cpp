#include "NightOwlEnginePCH.h"
#include "Texture2DComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"


dae::Texture2DComponent::Texture2DComponent(GameObject* pParentObject)
	:m_pParentObject{ nullptr }
{
	if (pParentObject)
	{
		m_pParentObject = pParentObject;
	}
}

void dae::Texture2DComponent::Update()
{
}

void dae::Texture2DComponent::Render() const
{
	if (m_Texture != nullptr)
	{
		const auto& pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

void dae::Texture2DComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}
