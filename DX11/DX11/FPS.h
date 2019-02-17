#pragma once

class FPS
{
	DELETE_ASSIGN_COPY( FPS );
public:
	FPS();
	~FPS();

	void Init();
	void Update();

	int GetFPS() const;

private:
	int m_FPS;
	int m_count;
	unsigned long m_startTime;
};