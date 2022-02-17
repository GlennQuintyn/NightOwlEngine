#include "NightOwlEnginePCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::GameObject()
	:m_Transform{}
	, m_pParent{ nullptr }
	, m_markForDelete{ false }
{
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaT)
{
	UNREFERENCED_PARAMETER(deltaT);

	if (m_markForDelete)
	{

	}

}

void dae::GameObject::FixedUpdate(float deltaT)
{
	UNREFERENCED_PARAMETER(deltaT);
}

void dae::GameObject::Render() const
{
	//const auto& pos = m_Transform.GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::SetParent(GameObject * parent)
{
	m_pParent = parent;
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return nullptr;
}

dae::GameObject* dae::GameObject::GetChildAt(int index) const
{
	//bound checking
	if (index < 0 || index >= m_pChildren.size())
		return nullptr;

	return m_pChildren[index].get();
}

void dae::GameObject::RemoveChild(int index)
{
	//bound checking
	if (index < 0 || index >= m_pChildren.size())
		return;

	//m_pChildren[index].release();
	m_pChildren[index].get()->m_markForDelete = true;
}

void dae::GameObject::AddChild(GameObject * object)
{
	UNREFERENCED_PARAMETER(object);
	//m_pChildren.push_back(std::make_unique<GameObject>(object));
}
