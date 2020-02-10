#pragma once

namespace GameDev2D
{
	class Timer;

	class TimerCallback
	{
	public:
		virtual ~TimerCallback() {}
		virtual void TimerIsDone(Timer* timer) = 0;
		virtual void TimerDidLoop(Timer* timer) = 0;
	};

	class Timer
	{
	public:
		Timer(double duration, bool startTimer = false, TimerCallback* callback = nullptr);
		Timer(double duration, TimerCallback* callback = nullptr);

		void Update(double delta);

		void Start();
		void Stop();

		void Reset(); //Resets a timer to zero but doesn't start it
		void Restart(); //Resets AND Starts the timer

		void SetDuration(double duration);
		double GetDuration();

		double GetElapsed();
		double GetRemaining();

		float GetPercentageElapsed();
		float GetPercentageRemaining();

		void SetDoesRepeat(bool doesRepeat);
		bool GetDoesRepeat();

		bool IsRunning();
		bool IsDone();

	private:
		double m_Duration;
		double m_Elapsed;
		bool m_IsRunning;
		bool m_DoesRepeat;
		TimerCallback* m_Callback;
	};
}