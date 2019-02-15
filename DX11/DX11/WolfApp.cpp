#include "pch.h"
#include "WolfApp.h"

WolfApp::WolfApp()
	: m_pKeyboard{ nullptr }
	, m_pRenderer{ nullptr }
{
	m_AppName = L"WindWolf";
	m_ClassName = L"WindWolf";
}
WolfApp::WolfApp( LPCWSTR AppName, LPCWSTR ClassName )
	: WolfApp()
{
	m_AppName = AppName;

	if( nullptr == ClassName )
		m_ClassName = m_AppName;
	else
		m_ClassName = ClassName;
}
WolfApp::~WolfApp()
{
}

bool WolfApp::Init()
{
	int width = 0, height = 0;
	InitWindow( width, height );

	m_pKeyboard = new Keyboard();
	if( nullptr == m_pKeyboard )
	{
		MessageBox( m_hWnd, L"InputClass Creating Fail", L"WolfApp::Initialize", MB_OK );
		return false;
	}
	m_pKeyboard->Init();

	m_pRenderer = new Renderer();
	if( nullptr == m_pRenderer )
	{
		MessageBox( m_hWnd, L"GraphcisClass Creating Fail", L"WolfApp::Initialize", MB_OK );
		return false;
	}
	if( !m_pRenderer->Init( width, height, m_hWnd ) )
	{
		MessageBox( m_hWnd, L"GraphcisClass Initialize Fail", L"WolfApp::Initialize", MB_OK );
		return false;
	}

	return true;
}

void WolfApp::Release()
{
	SAFE_TERMINATE( m_pRenderer );
	SAFE_DELETE( m_pKeyboard );

	TerminateWindow();
}

void WolfApp::Run()
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
			result = Update();
			if( !result )
				done = true;
		}
	}
}

LRESULT WolfApp::MessageHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
		case WM_KEYDOWN:
		{
			m_pKeyboard->KeyDown( (unsigned int)wParam );
			return 0;
		}

		case WM_KEYUP:
		{
			m_pKeyboard->KeyUp( (unsigned int)wParam );
			return 0;
		}

		default:
		{
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}
	}	// end switch
}

bool WolfApp::Update()
{
	if( m_pKeyboard->IsKeyDown( VK_ESCAPE ) )
		return false;

	if( m_pKeyboard->IsKeyDown( VK_UP ) )
		m_pRenderer->CameraUpdate( VK_UP );
	
	if( m_pKeyboard->IsKeyDown( VK_DOWN ) )
		m_pRenderer->CameraUpdate( VK_DOWN );

	if( m_pKeyboard->IsKeyDown( VK_LEFT ) )
		m_pRenderer->CameraUpdate( VK_LEFT );

	if( m_pKeyboard->IsKeyDown( VK_RIGHT ) )
		m_pRenderer->CameraUpdate( VK_RIGHT );

	if( !m_pRenderer->Update() )
		return false;

	return true;
}

void WolfApp::InitWindow( int & width, int & height )
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	m_hInstance = GetModuleHandle( NULL );

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
	wc.lpszClassName = m_ClassName;
	wc.cbSize = sizeof( WNDCLASSEX );

	RegisterClassEx( &wc );

	width = GetSystemMetrics( SM_CXSCREEN );
	height = GetSystemMetrics( SM_CYSCREEN );

	if( RendererValue::FULL_SCREEN )
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
	m_hWnd = CreateWindowEx( WS_EX_APPWINDOW, m_AppName, m_ClassName,
							 WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							 posX, posY, width, height, NULL, NULL, m_hInstance, NULL );

	gWolfApp = this;
	g_hWnd = m_hWnd;

	// Bring the window up on the screen and set it as main focus.
	ShowWindow( m_hWnd, SW_SHOW );
	SetForegroundWindow( m_hWnd );
	SetFocus( m_hWnd );
}

void WolfApp::TerminateWindow()
{
	if( RendererValue::FULL_SCREEN )
	{
		ChangeDisplaySettings( nullptr, 0 );
	}

	DestroyWindow( m_hWnd );
	m_hWnd = nullptr;

	UnregisterClass( m_ClassName, m_hInstance );
	gWolfApp = nullptr;
}

