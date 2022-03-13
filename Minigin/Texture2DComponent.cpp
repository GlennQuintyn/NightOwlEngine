#include "NightOwlEnginePCH.h"
#include "Texture2DComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

using namespace dae;

Texture2DComponent::Texture2DComponent(GameObject* pParentObject)
	:m_pParentObject{ nullptr }
{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("Texture2DComponent:\tPARENT OBJECT WAS NOT GIVEN!");
}

void Texture2DComponent::Update()
{
}

void Texture2DComponent::Render() const
{
	if (m_Texture != nullptr)
	{
		const auto& pos = m_pParentObject->GetWorldPosition();//m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

void Texture2DComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}
