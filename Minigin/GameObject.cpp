#include "NightOwlEnginePCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Scene.h"

using namespace dae;

unsigned int GameObject::m_ObjIdCounter = 0;

GameObject::GameObject(const std::string_view objectName, Scene* pScene, GameObject* pParent)
	: m_LocalTransform{}
	, m_WorldTransform{}
	, m_Name{}
	, m_pParent{ pParent }
	, m_pScene{ pScene }
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

	m_TransformIsDirty = true;
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
	//checks if anything needs updating and then updates it
	UpdateTransform();

	for (auto& component : m_pComponents)
	{
		component.first->Update();
	}

	for (auto& child : m_pChildren)
	{
		child->Update();
	}
}

void GameObject::FixedUpdate(float)
{
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

//TODO: Fix adding a child and parent code for the transform stuff

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

#pragma region transform code
void dae::GameObject::SetLocalTransform(const Transform & transform)
{
	m_LocalTransform = transform;
	m_TransformIsDirty = true;
}

void dae::GameObject::SetLocalPosition(float x, float y)
{
	m_LocalTransform.position.x = x;
	m_LocalTransform.position.y = y;
	m_PositionIsDirty = true;
}

void dae::GameObject::SetLocalPosition(const glm::vec2 & position)
{
	m_LocalTransform.position = position;
	m_PositionIsDirty = true;
}

void dae::GameObject::SetLocalRotation(float radians)
{
	m_LocalTransform.rotation = radians;
	m_RotationIsDirty = true;
}

void dae::GameObject::SetLocalScale(float x, float y)
{
	m_LocalTransform.scale.x = x;
	m_LocalTransform.scale.y = y;
	m_ScaleIsDirty = true;
}

void dae::GameObject::SetLocalScale(const glm::vec2 & scale)
{
	m_LocalTransform.scale = scale;
	m_ScaleIsDirty = true;
}

void dae::GameObject::SetLocalZDept(float z)
{
	m_LocalTransform.zDept = z;
	m_ZDeptIsDirty = true;
}

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

	//m_TransformIsDirty = false;
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

//
//std::vector<std::unique_ptr<dae::GameObject>>::iterator dae::GameObject::CheckNameDuplicateInChildren(const std::string & childName)
//{
//
//	auto it = std::find_if(m_pChildren.begin(), m_pChildren.end(), [&childName](const std::unique_ptr<GameObject>& child)
//		{
//			return child->GetName() == childName;
//		});
//
//
//	sizeof(std::vector<std::unique_ptr<GameObject>>::iterator)
//
//	//if the name was foudn in the list of children then the Iterator is not equal to the end
//	//return it != m_pChildren.end();
//	return it;
//}
