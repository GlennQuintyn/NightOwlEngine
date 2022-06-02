#pragma once
#include <vector>
#include <memory>

namespace dae
{
	class GameObject;
	class Observer;

	class Subject final
	{
	public:
		Subject() = default;
		~Subject() = default;

		void AddObserver(Observer& observer);
		void RemoveObserver(const Observer* observer);
		void Notify(GameObject* pObject, int event);

		Subject(const Subject& other) = delete;
		Subject(Subject&& other) = delete;
		Subject& operator=(const Subject& other) = delete;
		Subject& operator=(Subject&& other) = delete;

	private:
		std::vector<Observer*> m_pObservers;
	};
}