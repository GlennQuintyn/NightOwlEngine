#pragma once
#include "Singleton.h"

namespace dae
{
	class Time final : public Singleton<Time>
	{
	public:
		~Time() = default;

		void Update(float deltaT);
		const float GetDeltaT() const { return m_DeltaT; };
		const float GetTotalTimeElapsed() const { return m_TotalTimeElapse; };
		const float GetFPS() const { return 1.f / m_AvgerageDelataT; }

		//some code i wanted to try, optimistic way to get a more average
		//void SetFPSVolatility(int level) { m_DeltaTHistory.resize(level); };

	private:
		friend class Singleton<Time>;
		Time() = default;
		//std::vector<float> m_DeltaTHistory{ 0.f, 0.f };
		float m_DeltaT{};
		float m_TotalTimeElapse{};
		//Average deltaT is to smooth out the the fps value, instead of having it from the last frame which would be more volatile
		float m_AvgerageDelataT{};
	};
}