#pragma once

class InputDevice
{
	enum { KEY_MAX = 256, };

	DELETE_ASSIGN_COPY( InputDevice )
public:
	InputDevice();
	~InputDevice();

	bool Init( HINSTANCE hInstacne, HWND hWnd, int width, int height );
	void Terminate();

	bool Update();

	bool IsEscapePressed() const;
	void GetMousePosition( POINT& pos ) const;

	void KeyDown( unsigned int key );
	void KeyUp( unsigned int key );
	bool IsKeyDown( unsigned int key );

private:
	bool ReadKeyboared();
	bool ReadMouse();
	void ProcessInput();

private:
	bool m_keys[ KEY_MAX ];

	int m_screenWidth;
	int m_screenHeight;
	
	POINT m_mouse;
	HWND m_hWnd;

	IDirectInput8* m_pDirectInput;
	IDirectInputDevice8* m_pKeyboard;
	IDirectInputDevice8* m_pMouse;
	
	unsigned char m_keyboardState[ KEY_MAX ];
	DIMOUSESTATE m_mouseState;
};