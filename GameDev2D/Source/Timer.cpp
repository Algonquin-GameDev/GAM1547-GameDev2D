#include "Timer.h"

namespace GameDev2D
{
	Timer::Timer(double duration, bool startTimer) :
		m_Duration(duration),
		m_Elapsed(0.0),
		m_IsRunning(startTimer)
	{

	}

	void Timer::Update(double delta)
	{
		if (m_IsRunning == true)
		{
			m_Elapsed += delta;
			if (m_Elapsed > m_Duration)
			{
				m_Elapsed = m_Duration;
				m_IsRunning = false;
			}
		}
	}

	void Timer::Start()
	{
		m_IsRunning = true;
	}

	void Timer::Stop()
	{
		m_IsRunning = false;
	}

	void Timer::Reset()
	{
		m_IsRunning = false;
		m_Elapsed = 0.0;
	}

	void Timer::Restart()
	{
		m_IsRunning = true;
		m_Elapsed = 0.0;
	}

	void Timer::SetDuration(double duration)
	{
		m_Duration = duration;
	}

	double Timer::GetDuration()
	{
		return m_Duration;
	}

	double Timer::GetElapsed()
	{
		return m_Elapsed;
	}

	double Timer::GetRemaining()
	{
		return m_Duration - m_Elapsed;
	}

	float Timer::GetPercentageComplete()
	{
		return m_Elapsed / m_Duration;
	}

	bool Timer::IsRunning()
	{
		return m_IsRunning;
	}

	bool Timer::IsDone()
	{
		return m_Elapsed == m_Duration;
	}
}