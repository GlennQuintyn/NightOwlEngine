#pragma once
#include "Transform.h"
//#include "SceneObject.h"
#include "BaseComponent.h"
#include <vector>

//#include "Texture2DComponent.h"

namespace dae
{
	//class Texture2D;

	class GameObject final//: public SceneObject
	{
	public:
		GameObject(const std::string_view& objectName = {});
		virtual ~GameObject();

		void Update(float deltaT);
		void FixedUpdate(float deltaT);
		void Render() const;

		void SetPosition(float x, float y);

		template <typename T> T* AddComponent();
		template <typename T> void AddComponent(T* component);
		template <typename T> T* GetComponent() const;
		template <typename T> void RemoveComponent();

		void SetParent(GameObject* parent) { m_pParent = parent; };
		GameObject* GetParent() const { return m_pParent; };

		size_t GetChildCount() const { return m_pChildren.size(); };

		GameObject* GetChildAt(int index) const;
		GameObject* GetChildByName(const std::string_view& childName) const;

		void RemoveChildAt(int index);
		void RemoveChildByName(const std::string_view& childName);

		void AddChild(GameObject* object);
		GameObject* AddChild(const std::string_view& childName = {});

		const std::string& GetName() const { return m_Name; };
		void SetName(const std::string_view& name) { m_Name = name; };

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::string m_Name;

		std::vector<std::pair<std::unique_ptr<BaseComponent>, const std::type_info*>> m_pComponents;
		std::vector<std::unique_ptr<GameObject>> m_pChildren;

		GameObject* m_pParent;

		Transform m_Transform;

		static unsigned int m_ObjIdCounter;

		//  mmm, every gameobject has a texture? Is that correct?
		//std::shared_ptr<Texture2D> m_Texture{};
	};

	template<typename T>
	inline T* GameObject::AddComponent()
	{
		auto newComponent = new T{};
		m_pComponents.emplace_back(newComponent, &typeid(T));
		return newComponent;
	}

	template<typename T>
	void dae::GameObject::AddComponent(T* component)
	{
		if (component)
		{
			m_pComponents.emplace_back(component, &typeid(component));
		}
	}

	template<typename T>
	inline T* GameObject::GetComponent() const
	{
		//typeid is static so you can compare their pointers
		auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const auto& component)
			{
				return component.second == &typeid(T);
			});

		if (it != m_pComponents.end())
			return static_cast<T*>(it->first.get());
		else
			return nullptr;
	}

	template<typename T>
	inline void GameObject::RemoveComponent()
	{
		auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const auto& component)
			{
				return component.second == &typeid(T);
			});

		if (it != m_pComponents.end())
			it->first.reset();
	}
}
