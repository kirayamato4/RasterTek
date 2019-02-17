#pragma once

class WTimer
{
	DELETE_ASSIGN_COPY( WTimer );
public:
	WTimer();
	~WTimer();

	bool Init();
	void Update();

	float GetTime() const;

private:
	__int64 m_frequency;
	__int64 m_startTime;

	float m_tickPerMS;
	float m_frameTime;
};