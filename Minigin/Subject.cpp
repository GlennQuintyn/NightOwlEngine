#include "NightOwlEnginePCH.h"
#include "Subject.h"
#include "Observer.h"

using namespace dae;

void Subject::AddObserver(Observer& observer)
{
	/*std::find_if(m_pObservers.begin(),)*/
	m_pObservers.push_back(&observer);
}

void Subject::RemoveObserver(const Observer* observer)
{
	auto it = std::find(m_pObservers.begin(), m_pObservers.end(), observer);

	if (it != m_pObservers.end())
		m_pObservers.erase(it);
}

void Subject::Notify(GameObject* pObject, int event)
{
	for (auto& pObserver : m_pObservers)
	{
		pObserver->Notify(pObject, event);
	}
}
