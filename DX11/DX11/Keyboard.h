#pragma once

class Keyboard
{
	enum { KEY_MAX = 256, };

	DELETE_ASSIGN_COPY( Keyboard )
public:
	Keyboard();
	~Keyboard();

	void Init();

	void KeyDown( unsigned int key );
	void KeyUp( unsigned int key );
	bool IsKeyDown( unsigned int key );

private:
	bool m_keys[ KEY_MAX ];
};