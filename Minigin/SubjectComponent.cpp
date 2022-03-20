#include "NightOwlEnginePCH.h"
#include "SubjectComponent.h"
#include "Observer.h"

dae::SubjectComponent::SubjectComponent(GameObject* pParentObject)
	:m_pParentObject{ nullptr }
{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("FPSComponent:\tPARENT OBJECT WAS NOT GIVEN!");
}

void dae::SubjectComponent::AddObserver(Observer& observer)
{
	/*std::find_if(m_pObservers.begin(),)*/
	m_pObservers.push_back(&observer);
}

void dae::SubjectComponent::RemoveObserver(const Observer* observer)
{
	auto it = std::find(m_pObservers.begin(), m_pObservers.end(), observer);

	if (it != m_pObservers.end())
		m_pObservers.erase(it);
}

void dae::SubjectComponent::Notify(GameObject* pObject, int event)
{
	for (auto& pObserver : m_pObservers)
	{
		pObserver->Notify(pObject, event);
	}
}
