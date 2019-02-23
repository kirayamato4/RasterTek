#pragma once

class Keyboard
{
	enum { KEY_MAX = 256, };
	DELETE_ASSIGN_COPY( Keyboard );

public:
	Keyboard();
	~Keyboard();
	
	void KeyDown( unsigned int key );
	void KeyUp( unsigned int key );
	
	bool IsKeyDown( unsigned int key ) const;

private:
	bool m_keys[ KEY_MAX ];
};