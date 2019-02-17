#pragma once

#include <Pdh.h>

class CPU
{
	DELETE_ASSIGN_COPY( CPU );

public:
	CPU();
	~CPU();

	void Init();
	void Terminate();
	void Update();

	int GetCPUUsage() const;

private:
	bool m_bCanReadCPU;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;

	DWORD m_lastSampleTime;
	long m_CPUUsage;
};