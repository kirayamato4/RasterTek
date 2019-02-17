#include "pch.h"
#include "CPU.h"

CPU::CPU()
{
}

CPU::~CPU()
{
}

void CPU::Init()
{
	PDH_STATUS status;

	m_bCanReadCPU = true;

	status = PdhOpenQuery( nullptr, 0, &m_queryHandle );
	if( ERROR_SUCCESS != status )
		m_bCanReadCPU = false;

	status = PdhAddCounter( m_queryHandle, TEXT( "\\Processor(_Total)\\% processor time" ), 0, &m_counterHandle );
	if( ERROR_SUCCESS != status )
		m_bCanReadCPU = false;

	m_lastSampleTime = GetTickCount();
	m_CPUUsage = 0;
}

void CPU::Terminate()
{
	if( m_bCanReadCPU )
		PdhCloseQuery( m_queryHandle );
}

void CPU::Update()
{
	PDH_FMT_COUNTERVALUE value;

	if( m_bCanReadCPU )
	{
		DWORD currentTick = GetTickCount();
		if( currentTick > m_lastSampleTime + 1000 )
		{
			m_lastSampleTime = currentTick;
			PdhCollectQueryData( m_queryHandle );
			PdhGetFormattedCounterValue( m_counterHandle, PDH_FMT_LONG, NULL, &value );
			m_CPUUsage = value.longValue;
		}
	}
}

int CPU::GetCPUUsage() const
{
	int usage = 0;

	if( m_bCanReadCPU )
		usage = static_cast<int>( m_CPUUsage );

	return usage;
}
