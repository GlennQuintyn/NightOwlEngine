#include "NightOwlEnginePCH.h"
#include "Time.h"
#include <numeric>

void dae::Time::Update(float deltaT)
{
	m_DeltaT = deltaT;
	m_TotalTimeElapse += deltaT;

	m_AvgerageDelataT += deltaT;
	m_AvgerageDelataT /= 2.0f;

	//some code i wanted to try, optimistic way to get a more average
	/*for (size_t index = 0; index < (m_DeltaTHistory.size() - 1); index++)
	{
		m_DeltaTHistory[index] = m_DeltaTHistory[index + 1];
	}
	m_DeltaTHistory.back() = Time::GetInstance().GetDeltaT();

	float sum = std::reduce(m_DeltaTHistory.begin(), m_DeltaTHistory.end());
	sum /= m_DeltaTHistory.size();

	m_AvgerageDelataT = sum;*/
}

