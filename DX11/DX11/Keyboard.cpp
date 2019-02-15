#include "pch.h"
#include "Keyboard.h"

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::Init()
{
	memset( m_keys, 0, sizeof( bool ) * KEY_MAX );
}

void Keyboard::KeyDown( unsigned int key )
{
	m_keys[ key ] = true;
}

void Keyboard::KeyUp( unsigned int key )
{
	m_keys[ key ] = false;
}

bool Keyboard::IsKeyDown( unsigned int key )
{
	return m_keys[ key ];
}
