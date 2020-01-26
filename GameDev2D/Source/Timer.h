#pragma once

namespace GameDev2D
{
	class Timer
	{
	public:
		Timer(double duration, bool startTimer = false);

		void Update(double delta);

		void Start();
		void Stop();

		void Reset(); //Resets a timer to zero but doesn't start it
		void Restart(); //Resets AND Starts the timer

		void SetDuration(double duration);

		double GetDuration();
		double GetElapsed();
		double GetRemaining();

		float GetPercentageComplete();

		bool IsRunning();
		bool IsDone();

	private:
		double m_Duration;
		double m_Elapsed;
		bool m_IsRunning;
	};
}