#pragma once



#include "InputClass.h"
#include "GraphicsClass.h"

class SystemClass
{
	DELETE_ASSIGN_COPY( SystemClass )
public:
	SystemClass();
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	bool Frame();
	void InitializeWindows( int& width, int& height );
	void ShutdownWindows();

private:
	LPCWSTR m_appName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	InputClass* m_input;
	GraphicsClass* m_graphics;
};
static SystemClass* gSystemClass = nullptr;

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

	default :
	{
		if( gSystemClass )
			return gSystemClass->MessageHandler( hWnd, uMsg, wParam, lParam );
		else
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
	}	// end switch

	
}