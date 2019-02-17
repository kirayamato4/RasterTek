#include "pch.h"
#include "InputDevice.h"

InputDevice::InputDevice()
	: m_pDirectInput{ nullptr }
	, m_pKeyboard{ nullptr }
	, m_pMouse{ nullptr }
	, m_mouse{ 0, 0 }
{
	memset( m_keys, 0, sizeof( bool ) * KEY_MAX );
}

InputDevice::~InputDevice()
{
}

bool InputDevice::Init( HINSTANCE hInstacne, HWND hWnd, int width, int height )
{
	m_screenWidth = width;
	m_screenHeight = height;

	m_hWnd = hWnd;
	
	HRESULT hr = S_OK;
	hr = DirectInput8Create( hInstacne, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, nullptr );
	HR_CHECK_RETURN( hr, L"DirectInput8Create" );

	
	// KEYBOARD
	hr = m_pDirectInput->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, nullptr );
	HR_CHECK_RETURN( hr, L"CreateKeyboard" );

	hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard );
	HR_CHECK_RETURN( hr, L"SetDataFormat" );

	hr = m_pKeyboard->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE );
	HR_CHECK_RETURN( hr, L"SetCooperativeLevel" );

	hr = m_pKeyboard->Acquire();
	HR_CHECK_RETURN( hr, L"Acquire" );

	// MOUSE
	hr = m_pDirectInput->CreateDevice( GUID_SysMouse, &m_pMouse, nullptr );
	HR_CHECK_RETURN( hr, L"CreateMouse" );

	hr = m_pMouse->SetDataFormat( &c_dfDIMouse );
	HR_CHECK_RETURN( hr, L"SetDataFormat" );

	hr = m_pMouse->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	HR_CHECK_RETURN( hr, L"SetCooperativeLevel" );

	hr = m_pMouse->Acquire();
	HR_CHECK_RETURN( hr, L"Acquire" );

	return true;
}

void InputDevice::Terminate()
{
#define SAFE_UNACQUARE( x ) if( x ) { x->Unacquire(); x->Release(); x = nullptr; }
	SAFE_UNACQUARE( m_pMouse );
	SAFE_UNACQUARE( m_pKeyboard );
	SAFE_RELEASE( m_pDirectInput );
#undef SAFE_UNACQUARE
}

bool InputDevice::Update()
{
	if( !ReadKeyboared() )
		return false;

	if( !ReadMouse() )
		return false;

	ProcessInput();

	return true;
}

bool InputDevice::IsEscapePressed() const
{
	return ( m_keyboardState[ DIK_ESCAPE ] & 0x80 ) > 0;
}

void InputDevice::GetMousePosition( POINT & pos ) const
{
	pos = m_mouse;
}

void InputDevice::KeyDown( unsigned int key )
{
	m_keys[ key ] = true;
}

void InputDevice::KeyUp( unsigned int key )
{
	m_keys[ key ] = false;
}

bool InputDevice::IsKeyDown( unsigned int key )
{
	if( m_keyboardState[ key ] & 0x80 )
	{
		return true;
	}

	return false;
}

bool InputDevice::ReadKeyboared()
{
	HRESULT hr = S_OK;
	hr = m_pKeyboard->GetDeviceState( sizeof( m_keyboardState ), (LPVOID)&m_keyboardState );
	if( FAILED( hr ) )
	{
		if( DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr )
			m_pKeyboard->Acquire();
		else
			return false;
	}

	return true;
}

bool InputDevice::ReadMouse()
{
	HRESULT hr = S_OK;
	hr = m_pMouse->GetDeviceState( sizeof( DIMOUSESTATE ), (LPVOID)&m_mouseState );
	if( FAILED( hr ) )
	{
		if( DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr )
			m_pMouse->Acquire();
		else
			return false;
	}

	return true;
}

void InputDevice::ProcessInput()
{
	RECT rect;
	GetWindowRect( m_hWnd, &rect );
	
	POINT cursor;
	GetCursorPos( &cursor );

	m_mouse.x = cursor.x - rect.left + m_mouseState.lX;
	m_mouse.y = cursor.y - rect.top + m_mouseState.lY;

	if( cursor.x < rect.left ) m_mouse.x = 0;
	if( cursor.y < rect.top ) m_mouse.y = 0;

	if( cursor.x > rect.right) m_mouse.x = m_screenWidth;
	if( cursor.y > rect.bottom ) m_mouse.y = m_screenHeight;
}
