#include "pch.h"
#include "Position.h"

Position::Position()
{
	m_frameTime = 0.0f;
	m_rotateY = 0.0f;

	m_leftSpeed = 0.0f;
	m_rightSpeed = 0.0f;
}

Position::~Position()
{
}

void Position::SetFrameTime( float frameTime )
{
	m_frameTime = frameTime;
}

void Position::GetRotation( float & rotateY )
{
	rotateY = m_rotateY;
}

void Position::TurnLeft( bool bKeyDown )
{
	float delta = 0.0001f;
	float maxDelta = 0.0015f;

	if( bKeyDown )
	{
		m_leftSpeed += m_frameTime * delta;

		if( m_leftSpeed > ( m_frameTime * maxDelta ) )
			m_leftSpeed = m_frameTime * maxDelta;
	}
	else
	{
		m_leftSpeed -= m_frameTime * delta;

		if( m_leftSpeed < 0.0f )
			m_leftSpeed = 0.0f;
	}

	m_rotateY -= m_leftSpeed;
	if( m_rotateY < 0.0f )
		m_rotateY += 360.0f;
}

void Position::TurnRight( bool bKeyDown )
{
	float delta = 0.0001f;
	float maxDelta = 0.0015f;

	if( bKeyDown )
	{
		m_rightSpeed += m_frameTime * delta;

		if( m_rightSpeed >( m_frameTime * maxDelta ) )
			m_rightSpeed = m_frameTime * maxDelta;
	}
	else
	{
		m_rightSpeed -= m_frameTime * delta;

		if( m_rightSpeed < 0.0f )
			m_rightSpeed = 0.0f;
	}

	m_rotateY += m_rightSpeed;
	if( m_rotateY > 360.0f )
		m_rotateY -= 360.0f;
}
