#pragma once

#include "InputDevice.h"
#include "Renderer.h"
#include "WSound.h"

#include "FPS.h"
#include "CPU.h"
#include "Timer.h"
#include "Position.h"

class WolfApp
	: public Align16
{
	DELETE_ASSIGN_COPY( WolfApp )
public:
	WolfApp();
	WolfApp( LPCWSTR AppName, LPCWSTR ClassName = nullptr );
	~WolfApp();

	bool Init();
	void Release();
	void Run();

	LRESULT CALLBACK MessageHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	bool Update();
	void InitWindow( int& width, int& height );
	void TerminateWindow();

private:
	LPCWSTR m_AppName;
	LPCWSTR m_ClassName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	InputDevice* m_pInputDevice;
	Renderer* m_pRenderer;
	WSound* m_pSound;

	FPS*	m_pFPS;
	CPU*	m_pCPU;
	WTimer* m_pTimer;
	Position* m_pPosition;
};

static WolfApp* gWolfApp = nullptr;

static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage( 0 );
			return 0;
		}

		default:
		{
			if( gWolfApp )
				return gWolfApp->MessageHandler( hWnd, uMsg, wParam, lParam );
			else
				return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}
	}	// end switch
}