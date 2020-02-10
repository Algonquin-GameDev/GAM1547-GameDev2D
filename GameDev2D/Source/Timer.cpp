#include "Timer.h"

namespace GameDev2D
{
	Timer::Timer(double duration, bool startTimer, TimerCallback* callback) :
		m_Duration(duration),
		m_Elapsed(0.0),
		m_IsRunning(startTimer),
		m_DoesRepeat(false),
		m_Callback(callback)
	{

	}

	Timer::Timer(double duration, TimerCallback* callback) :
		m_Duration(duration),
		m_Elapsed(0.0),
		m_IsRunning(false),
		m_DoesRepeat(false),
		m_Callback(callback)
	{

	}

	void Timer::Update(double delta)
	{
		if (m_IsRunning == true)
		{
			m_Elapsed += delta;
			if (m_Elapsed > m_Duration)
			{
				if (m_DoesRepeat == true)
				{
					m_Elapsed -= m_Duration;

					if (m_Callback != nullptr)
					{
						m_Callback->TimerDidLoop(this);
					}
				}
				else
				{
					m_Elapsed = m_Duration;
					m_IsRunning = false;

					if (m_Callback != nullptr)
					{
						m_Callback->TimerIsDone(this);
					}
				}
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

	float Timer::GetPercentageElapsed()
	{
		return m_Elapsed / m_Duration;
	}

	float Timer::GetPercentageRemaining()
	{
		return 1.0f - GetPercentageElapsed();
	}

	void Timer::SetDoesRepeat(bool doesRepeat)
	{
		m_DoesRepeat = doesRepeat;
	}

	bool Timer::GetDoesRepeat()
	{
		return m_DoesRepeat;
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