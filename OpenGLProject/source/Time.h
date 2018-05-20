#pragma once

// singleton time manager
class Time
{
public:

	static Time& getInstance();

	void update();

	const float deltaTime() { return m_deltaTime; }

private:

	Time() {};
	~Time() {};

	float m_lastFrame = 0;
	float m_currentFrame = 0;
	float m_deltaTime = 0;

	float m_fps = 0;
};