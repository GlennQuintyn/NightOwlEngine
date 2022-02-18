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

	private:
		friend class Singleton<Time>;
		Time() = default;

		float m_DeltaT{};
		float m_TotalTimeElapse{};
		//Average deltaT is to smooth out the the fps value, instead of having it from the last frame which would be more volatile
		float m_AvgerageDelataT{};
	};
}