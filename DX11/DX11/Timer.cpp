#include "pch.h"
#include "Timer.h"

WTimer::WTimer()
{
}

WTimer::~WTimer()
{
}

bool WTimer::Init()
{
	if( !QueryPerformanceFrequency( (LARGE_INTEGER*)&m_frequency ) )
		return false;

	m_tickPerMS = m_frequency / 1000.0f;
	QueryPerformanceCounter( (LARGE_INTEGER*)&m_startTime );

	return true;
}

void WTimer::Update()
{
	__int64 currentTime;
	float elapsedTime;

	QueryPerformanceCounter( (LARGE_INTEGER*)&currentTime );

	elapsedTime = (float)( currentTime - m_startTime );
	m_frameTime = elapsedTime / m_tickPerMS;

	m_startTime = currentTime;
}

float WTimer::GetTime() const
{
	return m_frameTime;
}
