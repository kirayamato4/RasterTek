#include "pch.h"
#include "WSound.h"

WSound::WSound()
	: m_pDirectSound{ nullptr }
	, m_pPrimaryBuffer{ nullptr }
	, m_pSecondaryBuffer{ nullptr }
{
}

WSound::~WSound()
{
}

bool WSound::Init( HWND hWnd )
{
	if( !InitDirectSound( hWnd ) )
		return false;

	if( !LoadWaveFile( "Resource/yamaha.wav", &m_pSecondaryBuffer ) )
		return false;

	return PlayWaveFile();
}

void WSound::Terminate()
{
	TerminateWaveFile( &m_pSecondaryBuffer );

	TerminateDirectSound();
}

bool WSound::InitDirectSound( HWND hWnd )
{
	HRESULT hr = S_OK;
	hr = DirectSoundCreate8( nullptr, &m_pDirectSound, nullptr );
	HR_CHECK_RETURN( hr, L"DirectSoundCreat8" );

	hr = m_pDirectSound->SetCooperativeLevel( hWnd, DSSCL_PRIORITY );
	HR_CHECK_RETURN( hr, L"SetCooperativeLevel" );

	DSBUFFERDESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	bufferDesc.dwSize = sizeof( DSBUFFERDESC );
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = nullptr;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	hr = m_pDirectSound->CreateSoundBuffer( &bufferDesc, &m_pPrimaryBuffer, nullptr );
	HR_CHECK_RETURN( hr, L"CreateSoundBuffer" );

	WAVEFORMATEX waveFormat;
	ZeroMemory( &waveFormat, sizeof( waveFormat ) );
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = ( waveFormat.wBitsPerSample / 8 ) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	hr = m_pPrimaryBuffer->SetFormat( &waveFormat );
	HR_CHECK_RETURN( hr, L"SetFormat" );

	return true;
}

void WSound::TerminateDirectSound()
{
	SAFE_RELEASE( m_pPrimaryBuffer );
	SAFE_RELEASE( m_pDirectSound );
}

bool WSound::LoadWaveFile( char * soundFileName, IDirectSoundBuffer8 ** ppSoundBuffer )
{
	FILE* fp = nullptr;
	int error = fopen_s( &fp, soundFileName, "rb" );
	if( 0 != error )
		return false;

	WAVE_HEADER waveHeader;
	ZeroMemory( &waveHeader, sizeof( waveHeader ) );

	unsigned int count = fread( &waveHeader, sizeof( waveHeader ), 1, fp );
	if( 1 != count )
		return false;

	// Check chunkID RIFF format
	if( waveHeader.chunkID[ 0 ] != 'R' || waveHeader.chunkID[ 1 ] != 'I' ||
		waveHeader.chunkID[ 2 ] != 'F' || waveHeader.chunkID[ 3 ] != 'F' )
		return false;

	// Check WAVE format
	if( waveHeader.format[ 0 ] != 'W' || waveHeader.format[ 1 ] != 'A' ||
		waveHeader.format[ 2 ] != 'V' || waveHeader.format[ 3 ] != 'E' )
		return false;

	// Check fmt Format
	if( waveHeader.subChunkID[ 0 ] != 'f' || waveHeader.subChunkID[ 1 ] != 'm' ||
		waveHeader.subChunkID[ 2 ] != 't' || waveHeader.subChunkID[ 3 ] != ' ' )
		return false;

	if( WAVE_FORMAT_PCM != waveHeader.audioFormat )
		return false;

	if( 2 != waveHeader.numChannels )
		return false;

	if( 44100 != waveHeader.sampleRate )
		return false;

	if( 16 != waveHeader.bitsPerSample )
		return false;

	if( waveHeader.dataChunkID[ 0 ] != 'd' || waveHeader.dataChunkID[ 1 ] != 'a' ||
		waveHeader.dataChunkID[ 2 ] != 't' || waveHeader.dataChunkID[ 3 ] != 'a' )
		return false;

	WAVEFORMATEX waveFormat;
	ZeroMemory( &waveFormat, sizeof( waveFormat ) );
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = ( waveFormat.wBitsPerSample / 8 ) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	DSBUFFERDESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	bufferDesc.dwSize = sizeof( DSBUFFERDESC );
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer* pTempBuffer = nullptr;
	HRESULT hr = S_OK;
	hr= m_pDirectSound->CreateSoundBuffer( &bufferDesc, &pTempBuffer, nullptr );
	HR_CHECK_RETURN( hr, L"CreateSoundBuffer" );

	hr = pTempBuffer->QueryInterface( IID_IDirectSoundBuffer8, ( void** )&*ppSoundBuffer );
	HR_CHECK_RETURN( hr, L"QueryInterface" );

	SAFE_RELEASE( pTempBuffer );

	fseek( fp, sizeof( WAVE_HEADER ), SEEK_SET );
	unsigned char* waveData = new unsigned char[ waveHeader.dataSize ];
	count = fread( waveData, 1, waveHeader.dataSize, fp );
	if( count != waveHeader.dataSize )
		return false;

	fclose( fp );

	unsigned char* bufferPtr = nullptr;
	unsigned long bufferSize = 0;
	hr = ( *ppSoundBuffer )->Lock( 0, waveHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, nullptr, 0, 0 );
	HR_CHECK_RETURN( hr, L"DSOUND Lock" );

	memcpy( bufferPtr, waveData, waveHeader.dataSize );

	hr = ( *ppSoundBuffer )->Unlock( (void*)bufferPtr, bufferSize, nullptr, 0 );
	HR_CHECK_RETURN( hr, L"DSOUND Unlock" );

	SAFE_DELETE_ARRAY( waveData );

	return true;
}

void WSound::TerminateWaveFile( IDirectSoundBuffer8 ** ppSoundBuffer )
{
	if( *ppSoundBuffer )
	{
		SAFE_RELEASE( (*ppSoundBuffer) );
	}
}

bool WSound::PlayWaveFile()
{
	HRESULT hr = S_OK;
	hr = m_pSecondaryBuffer->SetCurrentPosition( 0 );
	HR_CHECK_RETURN( hr, L"SetCurrentPosition" );

	hr = m_pSecondaryBuffer->SetVolume( DSBVOLUME_MAX );
	HR_CHECK_RETURN( hr, L"SetVolume" );

	hr = m_pSecondaryBuffer->Play( 0, 0, DSBPLAY_LOOPING );
	HR_CHECK_RETURN( hr, L"Play" );

	return true;
}
