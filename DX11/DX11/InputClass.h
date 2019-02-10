#pragma once

class InputClass
{
	enum { KEY_MAX = 256, };

	DELETE_ASSIGN_COPY( InputClass )
public:
	InputClass();
	~InputClass();

	void Initialize();

	void KeyDown( unsigned int key );
	void KeyUp( unsigned int key );
	bool IsKeyDown( unsigned int key );

private:
	bool m_keys[ KEY_MAX ];
};