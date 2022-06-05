#include "NightOwlEnginePCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Scene.h"

using namespace dae;

unsigned int GameObject::m_ObjIdCounter = 0;

/*
Transform m_LocalTransform;
Transform m_WorldTransform;

std::string m_Name;

std::vector<std::pair<std::unique_ptr<BaseComponent>, const std::type_info*>> m_pComponents;
std::vector<std::unique_ptr<GameObject>> m_pChildren;

GameObject* m_pParent;
Scene* m_pScene;

static unsigned int m_ObjIdCounter;

bool m_TransformIsDirty;
bool m_PositionIsDirty;
bool m_RotationIsDirty;
bool m_ScaleIsDirty;
bool m_ZDeptIsDirty;*/

GameObject::GameObject(const std::string_view objectName, Scene* pScene, GameObject* pParent)
	: m_LocalTransform{}
	, m_WorldTransform{}
	, m_Name{}
	, m_pParent{ pParent }
	, m_pScene{ pScene }
	, m_Enabled{ true }//enabled by default
	, m_TransformIsDirty{ false }
	, m_PositionIsDirty{ false }
	, m_RotationIsDirty{ false }
	, m_ScaleIsDirty{ false }
	, m_ZDeptIsDirty{ false }
{
	m_ObjIdCounter++;

	//objects get a default name if nothing was given
	if (objectName.empty())
		m_Name = "Object" + std::to_string(m_ObjIdCounter);
	else
	{
		if (m_pParent)
		{
			//if the object is a child it can't have a scene as it is not a rootobject
			if (m_pScene)
			{
				m_pScene = nullptr;
				Logger::GetInstance().LogWarning("GAMEOBJECT:\tCHILD OBJECT CAN'T HAVE PTR TO SCENE!");
			}

			const auto& children = m_pParent->GetChildren();

			auto it = std::find_if(children.begin(), children.end(), [&objectName](const auto& child)
				{
					return child->GetName() == objectName;
				});

			//if duplicate was found, change name to auto generated name and log error
			if (it != children.end())
			{
				m_Name = "Object" + std::to_string(m_ObjIdCounter);
				Logger::GetInstance().LogWarning("GAMEOBJECT:\tDUPLICATE CHILD NAME WAS FOUND!\tNEW CHILD NAME IS " + m_Name);
			}
			else
				m_Name = objectName;
		}
		else
			m_Name = objectName;
	}

	//m_TransformIsDirty = true;
}

GameObject::~GameObject() = default;

#pragma region UpdateCode
void GameObject::LateInit()
{
	for (auto& component : m_pComponents)
	{
		component.first->LateInit();
	}

	for (auto& child : m_pChildren)
	{
		child->LateInit();
	}
}

void GameObject::Update()
{
	//if not enabled then it doesn't need to update and neither does its components and children
	if (!m_Enabled)
		return;

	for (auto& component : m_pComponents)
	{
		component.first->Update();
	}

	for (auto& child : m_pChildren)
	{
		child->Update();
	}
}

void GameObject::FixedUpdate(float deltaT)
{
	//if not enabled then it doesn't need to update and neither does its components and children
	if (!m_Enabled)
		return;

	for (auto& child : m_pChildren)
	{
		child->FixedUpdate(deltaT);
	}
}

void GameObject::LateUpdate()
{
	for (auto& component : m_pComponents)
	{
		component.first->LateUpdate();
	}

	for (auto& child : m_pChildren)
	{
		child->LateUpdate();
	}
}

void GameObject::Render() const
{
	//if not enabled then it doesn't need to rendered and neither does its components and children
	if (!m_Enabled)
		return;

	for (const auto& component : m_pComponents)
	{
		component.first->Render();
	}

	for (const auto& child : m_pChildren)
	{
		child->Render();
	}
}

void GameObject::Reset()
{
	for (const auto& component : m_pComponents)
	{
		component.first->Reset();
	}

	for (const auto& child : m_pChildren)
	{
		child->Reset();
	}
}
#pragma endregion

Scene* GameObject::GetScene()
{
	//if this gameobject doesn't have a scene then its not a rootobject so check its parent
	if (m_pScene)
		return m_pScene;
	else
		return m_pParent->GetScene();
}

void dae::GameObject::SetEnabledState(bool enabled)
{
	m_Enabled = enabled;

	for (auto& pChild : m_pChildren)
	{
		pChild->SetEnabledState(enabled);
	}
}

void GameObject::SetParent(GameObject * pParent, bool keepWorldPosition)
{
	//if the new parent is a nullptr
	if (!pParent)
	{
		//and it had a parent, remove it from its old parent and add it too the scene, if not then it was already a rootobject
		if (m_pParent)
		{
			auto childSelf = m_pParent->RemoveChildByName(m_Name);

			//get the scene from its former parent
			m_pScene = m_pParent->GetScene();
			m_pParent = nullptr;

			//hang it onto the scene now, because its a rootobject now
			m_pScene->TakeOwnership(std::move(childSelf));
		}

		SetLocalTransform(GetWorldTransform());
		//SetLocalPosition(GetWorldPosition());

		return;
	}

	//if the new parent isn't a nullptr, check if it's name already exists as one of its children
	const auto& children = pParent->GetChildren();

	//check if a child with the same name already exists
	auto it = std::find_if(children.begin(), children.end(), [this](const std::unique_ptr<GameObject>& child)
		{
			return child->GetName() == m_Name;
		});

	if (it != children.end())
	{
		Logger::GetInstance().LogError("GAMEOBJECT:\tPARENT " + pParent->GetName() + " ALREDY HAS CHILD WITH NAME " + m_Name);
		return;
	}

	//if it has a parent, remove this child from the old parent before adding to the new parent
	if (m_pParent)
	{
		//m_pParent->RemoveChildByName(m_Name);
		auto childSelf = m_pParent->RemoveChildByName(m_Name);

		//set parent to new parent and add this object as one of its children
		m_pParent = pParent;
		//set the scene to nullptr to show that it isn't a rootobject
		m_pScene = nullptr;

		m_pParent->AdoptChild(std::move(childSelf));
	}
	else
	{
		auto childSelf = m_pScene->ReleaseOwnership(*this);

		m_pParent = pParent;
		m_pScene = nullptr;

		m_pParent->AdoptChild(std::move(childSelf));
	}

	if (keepWorldPosition)
	{
		SetLocalTransform(m_LocalTransform - m_pParent->GetWorldTransform());
		m_TransformIsDirty = true;
	}
}

#pragma region ChildCode
GameObject* GameObject::GetChildAt(int index) const
{
	//bound checking
	if (index < 0 || size_t(index) >= m_pChildren.size())
		return nullptr;

	return m_pChildren[index].get();
}

GameObject* GameObject::GetChildByName(const std::string_view childName) const
{
	auto it = std::find_if(m_pChildren.begin(), m_pChildren.end(), [&childName](const std::unique_ptr<GameObject>& child)
		{
			return child->GetName() == childName;
		});

	if (it != m_pChildren.end())
		return it->get();

	return nullptr;
}

std::unique_ptr<GameObject> GameObject::RemoveChildAt(size_t index)
{
	//bound checking
	if (index < 0 || index >= m_pChildren.size())
	{
		Logger::GetInstance().LogWarning("GAMEOBJECT:\tINDEX WAS OUT OF BOUNDS!");
		return {};//out of bound so returing empty uniqueptr
	}

	std::unique_ptr<GameObject> child = std::move(m_pChildren[index]);
	m_pChildren.erase(m_pChildren.begin() + index);
	return child;
}

std::unique_ptr<GameObject> GameObject::RemoveChildByName(const std::string_view childName)
{
	auto it = std::find_if(m_pChildren.begin(), m_pChildren.end(), [&childName](const std::unique_ptr<GameObject>& child)
		{
			return child->GetName() == childName;
		});

	if (it != m_pChildren.end())
	{
		return RemoveChildAt(std::distance(m_pChildren.begin(), it));
	}
	else
	{
		std::stringstream errormsg{};
		errormsg << "GAMEOBJECT:\tCHILD WITH NAME " << childName << " WAS NOT FOUND!";
		Logger::GetInstance().LogWarning(errormsg.view());
		return {};
	}
}

void GameObject::AdoptChild(std::unique_ptr<GameObject> pObject)
{
	//if the given object already has a parent then it can't be childed to another object
	//if (pObject->GetParent())
	//{
	//	Logger::GetInstance().LogError("GAMEOBJECT:\tOBJECT CAN'T BE CHILDED TWICE!");
	//	return;
	//}

	//pObject->SetParent(this);
	m_pChildren.push_back(std::move(pObject));
}

GameObject& GameObject::CreateAddChild(const std::string_view childName)
{
	auto newObject = new GameObject{ childName, nullptr, this };
	m_pChildren.emplace_back(newObject);
	return *m_pChildren.back();
}
#pragma endregion

#pragma region Transform_Code
#pragma region GetWorldTransforms
const Transform& dae::GameObject::GetWorldTransform()
{
	if (m_TransformIsDirty)
	{
		//checks if anything needs updating and then updates it
		UpdateTransform();
	}

	return m_WorldTransform;
}

const glm::vec2& dae::GameObject::GetWorldPosition()
{
	if (m_PositionIsDirty)
	{
		UpdatePosition();
	}

	return m_WorldTransform.position;
}

const float dae::GameObject::GetWorldRotation()
{
	if (m_RotationIsDirty)
	{
		UpdateRotation();
	}

	return m_WorldTransform.rotation;
}

const glm::vec2& dae::GameObject::GetWorldScale()
{
	if (m_ScaleIsDirty)
	{
		UpdateScale();
	}

	return m_WorldTransform.scale;
}

const float dae::GameObject::GetWorldZDept()
{
	if (m_ZDeptIsDirty)
	{
		UpdateZDept();
	}

	return m_WorldTransform.zDept;
}
#pragma endregion

#pragma region SettingLocalTransform
void dae::GameObject::SetLocalTransform(const Transform & transform)
{
	m_LocalTransform = transform;
	SetTransformsDirty();
}

void dae::GameObject::SetLocalPosition(float x, float y)
{
	m_LocalTransform.position.x = x;
	m_LocalTransform.position.y = y;
	SetPositionsDirty();
}

void dae::GameObject::SetLocalPosition(const glm::vec2 & position)
{
	m_LocalTransform.position = position;
	SetPositionsDirty();
}

void dae::GameObject::SetLocalRotation(float radians)
{
	m_LocalTransform.rotation = radians;
	SetRotationsDirty();
}

void dae::GameObject::SetLocalScale(float x, float y)
{
	m_LocalTransform.scale.x = x;
	m_LocalTransform.scale.y = y;
	SetScalesDirty();
}

void dae::GameObject::SetLocalScale(const glm::vec2 & scale)
{
	m_LocalTransform.scale = scale;
	SetScalesDirty();
}

void dae::GameObject::SetLocalZDept(float z)
{
	m_LocalTransform.zDept = z;
	SetZDeptsDirty();
}
#pragma endregion

#pragma region SettingDirty
void dae::GameObject::SetTransformsDirty()
{
	//if object was already dirty then no need to set itself and its children dirty as they are already dirty
	if (m_TransformIsDirty)
		return;

	m_TransformIsDirty = true;

	for (auto& pChild : m_pChildren)
		pChild->SetTransformsDirty();
}

void dae::GameObject::SetPositionsDirty()
{
	//if object was already dirty then no need to set itself and its children dirty as they are already dirty
	if (m_PositionIsDirty)
		return;

	m_PositionIsDirty = true;

	for (auto& pChild : m_pChildren)
		pChild->SetPositionsDirty();
}

void dae::GameObject::SetRotationsDirty()
{
	//if object was already dirty then no need to set itself and its children dirty as they are already dirty
	if (m_RotationIsDirty)
		return;

	m_RotationIsDirty = true;

	for (auto& pChild : m_pChildren)
		pChild->SetRotationsDirty();
}

void dae::GameObject::SetScalesDirty()
{
	//if object was already dirty then no need to set itself and its children dirty as they are already dirty
	if (m_ScaleIsDirty)
		return;

	m_ScaleIsDirty = true;

	for (auto& pChild : m_pChildren)
		pChild->SetScalesDirty();
}

void dae::GameObject::SetZDeptsDirty()
{
	//if object was already dirty then no need to set itself and its children dirty as they are already dirty
	if (m_ZDeptIsDirty)
		return;

	m_ZDeptIsDirty = true;

	for (auto& pChild : m_pChildren)
		pChild->SetZDeptsDirty();
}
#pragma endregion

#pragma region UpdateTransform
void GameObject::UpdateTransform()
{
	if (m_PositionIsDirty || m_TransformIsDirty)
	{
		UpdatePosition();
	}
	if (m_RotationIsDirty || m_TransformIsDirty)
	{
		UpdateRotation();
	}
	if (m_ScaleIsDirty || m_TransformIsDirty)
	{
		UpdateScale();
	}
	if (m_ZDeptIsDirty || m_TransformIsDirty)
	{
		UpdateZDept();
	}

	m_TransformIsDirty = false;
}

void GameObject::UpdatePosition()
{
	if (!m_pParent)
		m_WorldTransform.position = m_LocalTransform.position;
	else
		m_WorldTransform.position = m_pParent->GetWorldPosition() + m_LocalTransform.position;

	m_PositionIsDirty = false;
}

void GameObject::UpdateRotation()
{
	if (!m_pParent)
		m_WorldTransform.rotation = m_LocalTransform.rotation;
	else
		m_WorldTransform.rotation = m_pParent->GetWorldRotation() + m_LocalTransform.rotation;

	m_RotationIsDirty = false;
}

void GameObject::UpdateScale()
{
	if (!m_pParent)
		m_WorldTransform.scale = m_LocalTransform.scale;
	else
		m_WorldTransform.scale = m_pParent->GetWorldScale() * m_LocalTransform.scale;

	m_ScaleIsDirty = false;
}

void GameObject::UpdateZDept()
{
	if (!m_pParent)
		m_WorldTransform.zDept = m_LocalTransform.zDept;
	else
		m_WorldTransform.zDept = m_pParent->GetWorldZDept() + m_LocalTransform.zDept;

	m_ZDeptIsDirty = false;
}
#pragma endregion
#pragma endregion