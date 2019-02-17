#include "pch.h"
#include "FPS.h"

FPS::FPS()
{
}

FPS::~FPS()
{
}

void FPS::Init()
{
	m_FPS = 0;
	m_count = 0;
	m_startTime = ::timeGetTime();
}

void FPS::Update()
{
	++m_count;

	DWORD currentTime = ::timeGetTime();
	if( currentTime >= ( m_startTime + 1000 ) )
	{
		m_FPS = m_count;
		m_count = 0;

		m_startTime = currentTime;
	}
}

int FPS::GetFPS() const
{
	return m_FPS;
}
