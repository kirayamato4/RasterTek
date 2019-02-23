#pragma once

class Position
{
	DELETE_ASSIGN_COPY( Position );
public:
	Position();
	~Position();

	void SetFrameTime( float delta );
	void GetRotation( float& rotateY );

	void TurnLeft( bool bKeyDown );
	void TurnRight( bool bKeyDown );

private:
	float m_frameTime;
	float m_rotateY;
	float m_leftSpeed;
	float m_rightSpeed;
};