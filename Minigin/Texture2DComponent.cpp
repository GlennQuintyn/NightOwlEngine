#include "NightOwlEnginePCH.h"
#include "Texture2DComponent.h"
#include "ResourceManager.h"

//#include "Renderer.h"

dae::Texture2DComponent::Texture2DComponent()
{
}

dae::Texture2DComponent::~Texture2DComponent()
{
}

void dae::Texture2DComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}
