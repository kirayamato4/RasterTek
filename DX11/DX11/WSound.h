#pragma once

class WSound
{
	DELETE_ASSIGN_COPY( WSound );
public:
	WSound();
	~WSound();

	bool Init( HWND hWnd );
	void Terminate();

private:
	bool InitDirectSound( HWND hWnd );
	void TerminateDirectSound();

	bool LoadWaveFile( char* soundFileName, IDirectSoundBuffer8** ppSoundBuffer );
	void TerminateWaveFile( IDirectSoundBuffer8** ppSoundBuffer );

	bool PlayWaveFile();

private:
	IDirectSound8* m_pDirectSound;
	IDirectSoundBuffer* m_pPrimaryBuffer;
	IDirectSoundBuffer8* m_pSecondaryBuffer;
};