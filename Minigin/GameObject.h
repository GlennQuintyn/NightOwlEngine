#pragma once
#include "BaseComponent.h"
#include "Transform.h"
#include <vector>

namespace dae
{
	class Scene;

	class GameObject final
	{
	public:
		GameObject(const std::string_view objectName = {}, Scene* pScene = nullptr, GameObject* pParent = nullptr);
		virtual ~GameObject();

		void LateInit();
		void Update();
		void FixedUpdate(float deltaT);
		void LateUpdate();
		void Render() const;

		void Reset();

		template <typename T> T& AddComponent();
		template <typename T> void AddComponent(T* component);
		template <typename T> T* GetComponent() const;
		template <typename T> void RemoveComponent();

		void SetLocalTransform(const Transform& transform);
		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec2& position);
		void SetLocalRotation(float radians);
		void SetLocalScale(float x, float y);
		void SetLocalScale(const glm::vec2& scale);
		void SetLocalZDept(float z);

		const Transform& GetLoacalTransform() const { return m_LocalTransform; };
		const glm::vec2& GetLocalPosition() const { return m_LocalTransform.position; };
		const float GetLoacalRotation() const { return m_LocalTransform.rotation; };
		const glm::vec2& GetLoacalScale() const { return m_LocalTransform.scale; };
		const float GetLoacalZDept() const { return m_LocalTransform.zDept; };

		const Transform& GetWorldTransform();
		const glm::vec2& GetWorldPosition();
		const float GetWorldRotation();
		const glm::vec2& GetWorldScale();
		const float GetWorldZDept();

		Scene* GetScene();

		void SetParent(GameObject* parent, bool keepWorldPosition = true);
		GameObject* GetParent() const { return m_pParent; };

		size_t GetChildCount() const { return m_pChildren.size(); };
		GameObject* GetChildAt(int index) const;
		GameObject* GetChildByName(const std::string_view childName) const;
		const std::vector<std::unique_ptr<GameObject>>& GetChildren() const { return m_pChildren; };

		std::unique_ptr<GameObject> RemoveChildAt(size_t index);
		std::unique_ptr<GameObject> RemoveChildByName(const std::string_view childName);

		GameObject& CreateAddChild(const std::string_view childName = {});

		const std::string& GetName() const { return m_Name; };
		//void SetName(const std::string_view name) { m_Name = name; };//TODO: implement properly in future, so that duplicate child naming is not possible

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void AdoptChild(std::unique_ptr<GameObject> pObject);

		void UpdateTransform();
		void UpdatePosition();
		void UpdateRotation();
		void UpdateScale();
		void UpdateZDept();

		void SetTransformsDirty();
		void SetPositionsDirty();
		void SetRotationsDirty();
		void SetScalesDirty();
		void SetZDeptsDirty();

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
		bool m_ZDeptIsDirty;
	};

#pragma region Templated Functions
	template<typename T>
	inline T& GameObject::AddComponent()
	{
		auto newComponent = new T{ this };
		m_pComponents.emplace_back(newComponent, &typeid(T));
		return *newComponent;
	}

	template<typename T>
	void dae::GameObject::AddComponent(T* component)
		//void dae::GameObject::AddComponent(BaseComponent* component, const std::type_info& compTypeId)
	{
		if (component)
			m_pComponents.emplace_back(component, &typeid(component));
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
#pragma endregion
}
