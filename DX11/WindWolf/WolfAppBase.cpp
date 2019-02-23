#include "pch.h"
#include "WolfAppBase.h"

WolfAppBase::WolfAppBase( int width, int height, std::wstring appName, std::wstring className )
{
	m_width = width;
	m_height = height;

	if( appName.length() > 0 ) 
		m_appName = appName;

	if( className.length() > 0 )
		m_className = className;
}

WolfAppBase::~WolfAppBase()
{
}

bool WolfAppBase::Init()
{
	if( !InitWindow() )
		return false;

	m_pKeyboard = new Keyboard;
	if( !m_pKeyboard )
		return false;

	m_pRenderer = new Renderer;
	if( !m_pRenderer )
		return false;

	if( !m_pRenderer->Init( m_width, m_height, m_hWnd ) )
		return false;

	return true;
}

void WolfAppBase::Terminate()
{
	SAFE_TERMINATE( m_pRenderer );
	SAFE_DELETE( m_pKeyboard );

	TerminateWindow();
}

void WolfAppBase::Run()
{
	MSG msg = { 0 };

	bool done = false;
	while( true )
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if( WM_QUIT == msg.message )
			break;

		if( !Update() )
			break;
	}
}

LRESULT WolfAppBase::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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
	}
}

bool WolfAppBase::Update()
{
	if( m_pKeyboard->IsKeyDown( VK_ESCAPE ) )
		return false;

	if( !m_pRenderer->Update() )
		return false;

	return true;
}

bool WolfAppBase::InitWindow()
{
	m_hInstance = GetModuleHandle( NULL );
	
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = ::MessageProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon( NULL, IDI_WINLOGO );
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_className.c_str();
	wc.cbSize = sizeof( WNDCLASSEX );
	RegisterClassEx( &wc );

	int screenWidth = GetSystemMetrics( SM_CXSCREEN );
	int screenHeight = GetSystemMetrics( SM_CYSCREEN );

	int x = static_cast<int>( ( screenWidth - m_width ) / 2 );
	int y = static_cast<int>( ( screenHeight - m_height ) / 2 );

	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		m_className.c_str(),
		m_appName.c_str(),
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION,
		x,
		y,
		m_width,
		m_height,
		nullptr,
		nullptr,
		m_hInstance,
		nullptr
	);

	if( !m_hWnd )
		return false;

	gWolfApp = this;
	
	ShowWindow( m_hWnd, SW_SHOW );
	SetForegroundWindow( m_hWnd );
	SetFocus( m_hWnd );

	return true;
}

void WolfAppBase::TerminateWindow()
{

	DestroyWindow( m_hWnd );
	m_hWnd = nullptr;

	UnregisterClass( m_className.c_str(), m_hInstance );
	gWolfApp = nullptr;
}

LRESULT CALLBACK MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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
				return gWolfApp->MessageProc( hWnd, uMsg, wParam, lParam );
			else
				return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}
	}	// end switch
}
