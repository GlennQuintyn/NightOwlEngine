#pragma once
#include "BaseComponent.h"
//#include "Observer.h"
#include <vector>
#include <memory>

namespace dae
{
	class GameObject;
	class Observer;

	class SubjectComponent final : public BaseComponent
	{
	public:
		SubjectComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		SubjectComponent(std::nullptr_t) = delete;

		~SubjectComponent() = default;

		void LateInit() override {};
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override {};

		void AddObserver(Observer& observer);
		void RemoveObserver(const Observer* observer);
		void Notify(GameObject* pObject, int event);

		SubjectComponent(const SubjectComponent& other) = delete;
		SubjectComponent(SubjectComponent&& other) = delete;
		SubjectComponent& operator=(const SubjectComponent& other) = delete;
		SubjectComponent& operator=(SubjectComponent&& other) = delete;

	private:
		std::vector<Observer*> m_pObservers;
		GameObject* m_pParentObject;

	};
}