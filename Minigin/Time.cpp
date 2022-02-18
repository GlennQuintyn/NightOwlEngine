#include "NightOwlEnginePCH.h"
#include "Time.h"

void dae::Time::Update(float deltaT)
{
	m_DeltaT = deltaT;
	m_TotalTimeElapse += deltaT;

	m_AvgerageDelataT += deltaT;
	m_AvgerageDelataT /= 2.0f;
}

