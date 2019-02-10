#include "pch.h"
#include "InputClass.h"

InputClass::InputClass()
{
}

InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	memset( m_keys, 0, sizeof( bool ) * KEY_MAX );
}

void InputClass::KeyDown( unsigned int key )
{
	m_keys[ key ] = true;
}

void InputClass::KeyUp( unsigned int key )
{
	m_keys[ key ] = false;
}

bool InputClass::IsKeyDown( unsigned int key )
{
	return m_keys[ key ];
}
