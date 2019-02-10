#include "pch.h"
#include "SystemClass.h"

SystemClass::SystemClass()
	: m_input{ nullptr }
	, m_graphics{ nullptr }
{
}
SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int width = 0, height = 0;
	InitializeWindows( width, height );

	m_input = new InputClass();
	if( nullptr == m_input )
	{
		MessageBox( m_hWnd, L"InputClass Creating Fail", L"SystemClass::Initialize", MB_OK );
		return false;
	}
	m_input->Initialize();

	m_graphics = new GraphicsClass();
	if( nullptr == m_graphics )
	{
		MessageBox( m_hWnd, L"GraphcisClass Creating Fail", L"SystemClass::Initialize", MB_OK );
		return false;
	}
	if( !m_graphics->Initialize( width, height, m_hWnd ) )
	{
		MessageBox( m_hWnd, L"GraphcisClass Initialize Fail", L"SystemClass::Initialize", MB_OK );
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	if( m_graphics )
	{
		m_graphics->Shutdown();
		delete m_graphics;
		m_graphics = nullptr;
	}

	if( m_input )
	{
		delete m_input;
		m_input = nullptr;
	}

	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg = { 0 };
	bool done = false, result;

	while( !done )
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if( WM_QUIT == msg.message )
		{
			done = true;
		}
		else
		{
			result = Frame();
			if( !result )
				done = true;
		}
	}
}

LRESULT SystemClass::MessageHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN:
	{
		m_input->KeyDown( (unsigned int)wParam );
		return 0;
	}

	case WM_KEYUP:
	{

		m_input->KeyUp( (unsigned int)wParam );
		return 0;
	}

	default:
	{
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
	}	// end switch
}

bool SystemClass::Frame()
{
	if( m_input->IsKeyDown( VK_ESCAPE ) )
		return false;

	if( !m_graphics->Frame() )
		return false;

	return true;
}

void SystemClass::InitializeWindows( int & width, int & height )
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	gSystemClass = this;

	m_hInstance = GetModuleHandle( NULL );

	m_appName = L"Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon( NULL, IDI_WINLOGO );
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_appName;
	wc.cbSize = sizeof( WNDCLASSEX );

	RegisterClassEx( &wc );

	width = GetSystemMetrics( SM_CXSCREEN );
	height = GetSystemMetrics( SM_CYSCREEN );

	if( GCV::FULL_SCREEN )
	{
		memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );
		dmScreenSettings.dmSize = sizeof( dmScreenSettings );
		dmScreenSettings.dmPelsWidth = (unsigned long)width;
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN );

		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		width = 800;
		height = 600;

		// Place the window in the middle of the screen.
		posX = ( GetSystemMetrics( SM_CXSCREEN ) - width ) / 2;
		posY = ( GetSystemMetrics( SM_CYSCREEN ) - height ) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hWnd = CreateWindowEx( WS_EX_APPWINDOW, m_appName, m_appName,
							 WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							 posX, posY, width, height, NULL, NULL, m_hInstance, NULL );

	// Bring the window up on the screen and set it as main focus.
	ShowWindow( m_hWnd, SW_SHOW );
	SetForegroundWindow( m_hWnd );
	SetFocus( m_hWnd );

	// Hide the mouse cursor.
	// ShowCursor( false );
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	// ShowCursor( true );

	if( GCV::FULL_SCREEN )
	{
		ChangeDisplaySettings( nullptr, 0 );
	}

	DestroyWindow( m_hWnd );
	m_hWnd = nullptr;

	UnregisterClass( m_appName, m_hInstance );
	gSystemClass = nullptr;
}

