#pragma once

class Time
{
private:
	float m_time;
	float m_prevTime = 0;

public:
	Time(float time = 0.0f)
		: m_time(time)
	{
	}

	/**
	 * \brief 
	 */
	operator float() const { return m_time; }

	void UpdateTime(float newTime)
	{
		m_prevTime = m_time;
		m_time = newTime;
	}

	float GetSeconds() const { return m_time; }
	float GetDeltaSeconds() const { return (m_time - m_prevTime); }

	float GetMilliseconds() const { return m_time * 1000.0f; }
	float GetDeltaMilliseconds() const { return (m_time - m_prevTime) * 1000.0f; }

};
