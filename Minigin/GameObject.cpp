#include "NightOwlEnginePCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

unsigned int dae::GameObject::m_ObjIdCounter = 0;

dae::GameObject::GameObject(const std::string_view& objectName)
	: m_Name{}
	, m_Transform{}
	, m_pParent{ nullptr }
{
	m_ObjIdCounter++;

	//objects get a default name if nothing was given
	if (objectName.empty())
		m_Name = "Object" + std::to_string(m_ObjIdCounter);
	else
		m_Name = objectName;

	//int* idk = new int;
	//idk;
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaT)
{
	UNREFERENCED_PARAMETER(deltaT);
	for (auto& component : m_pComponents)
	{
		component.first->Update();
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
	for (const auto& component : m_pComponents)
	{
		component.first->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

dae::GameObject* dae::GameObject::GetChildAt(int index) const
{
	//bound checking
	if (index < 0 || size_t(index) >= m_pChildren.size())
		return nullptr;

	return m_pChildren[index].get();
}

dae::GameObject* dae::GameObject::GetChildByName(const std::string_view & childName) const
{
	auto it = std::find_if(m_pChildren.begin(), m_pChildren.end(), [&childName](const std::unique_ptr<GameObject>& child)
		{
			return child->GetName() == childName;
		});

	if (it != m_pChildren.end())
		return it->get();

	return nullptr;
}

void dae::GameObject::RemoveChildAt(int index)
{
	//bound checking
	if (index < 0 || size_t(index) >= m_pChildren.size())
		return;

	m_pChildren.erase(m_pChildren.begin() + index);
}

void dae::GameObject::RemoveChildByName(const std::string_view & childName)
{
	auto it = std::find_if(m_pChildren.begin(), m_pChildren.end(), [&childName](const std::unique_ptr<GameObject>& child)
		{
			return child->GetName() == childName;
		});


	if (it != m_pChildren.end())
		m_pChildren.erase(it);
}

void dae::GameObject::AddChild(GameObject * object)
{
	object->SetParent(this);
	m_pChildren.emplace_back(object);
}

dae::GameObject* dae::GameObject::AddChild(const std::string_view & childName)
{
	auto newObject = new GameObject{ childName };
	newObject->SetParent(this);
	m_pChildren.emplace_back(newObject);
	return newObject;
}
