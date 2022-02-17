#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "BaseComponent.h"
#include <vector>

namespace dae
{
	class Texture2D;

	class GameObject final//: public SceneObject
	{
	public:
		GameObject();
		virtual ~GameObject();

		void Update(float deltaT);
		void FixedUpdate(float deltaT);
		void Render() const;

		void SetPosition(float x, float y);

		template <typename T> T* AddComponent();
		template <typename T> T* GetComponent() const;
		template <typename T> void RemoveComponent();

		void SetParent(GameObject* parent);
		GameObject* GetParent() const;

		size_t GetChildCount() const { return m_pChildren.size(); };
		GameObject* GetChildAt(int index) const;
		//todo: add func to remove child by name AND add string m_pName to all objects and static counter to make default name
		void RemoveChild(int index);
		void AddChild(GameObject* object);

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_Transform;

		std::vector<std::unique_ptr<BaseComponent>> m_pComponents;
		std::vector<std::unique_ptr<GameObject>> m_pChildren;

		GameObject* m_pParent;

		bool m_markForDelete;
		//GameObject* m_pChild;

		//  mmm, every gameobject has a texture? Is that correct?
		//std::shared_ptr<Texture2D> m_Texture{};
	};

	template<typename T>
	inline T* GameObject::AddComponent()
	{
		m_pComponents.push_back(std::make_unique<T>());
		return m_pComponents.back();
	}

	template<typename T>
	inline T* GameObject::GetComponent() const
	{
		//todo: add loop to search for firt occerence of type
		return nullptr;
	}

	template<typename T>
	inline void GameObject::RemoveComponent()
	{
		//todo: add loop to search for firt occerence of type

	}
}
