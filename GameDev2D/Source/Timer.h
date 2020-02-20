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
		Timer(double duration, bool startTimer, TimerCallback* callback = nullptr);
		Timer(double duration, TimerCallback* callback = nullptr);

		void Update(double delta);

		void Start();
		void Stop();

		void Reset(); //Resets a timer to zero but doesn't start it
		void Restart(); //Resets AND Starts the timer

		void SetDuration(double duration);
		double GetDuration();

		void SetCallback(TimerCallback* callback);
		TimerCallback* GetCallback();

		double GetElapsed();
		double GetRemaining();

		float GetPercentageElapsed();
		float GetPercentageRemaining();

		void SetDoesLoop(bool doesLoop);
		bool GetDoesLoop();

		bool IsRunning();
		bool IsDone();

	private:
		double m_Duration;
		double m_Elapsed;
		bool m_IsRunning;
		bool m_DoesLoop;
		TimerCallback* m_Callback;
	};
}