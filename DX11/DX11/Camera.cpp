#include "pch.h"
#include "Camera.h"

Camera::Camera()
	: m_px{ 0.0f }
	, m_py{ 0.0f }
	, m_pz{ 0.0f }
	, m_rx{ 0.0f }
	, m_ry{ 0.0f }
	, m_rz{ 0.0f }
{
}

Camera::~Camera()
{
}

void Camera::SetPosition( float x, float y, float z )
{
	m_px = x;
	m_py = y;
	m_pz = z;

	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

void Camera::SetRotation( float x, float y, float z )
{
	m_rx = x;
	m_ry = y;
	m_rz = z;

	m_rot.x = x;
	m_rot.y = y;
	m_rot.z = z;
}

XMFLOAT3 Camera::GetPosition() const
{
	return m_pos;
}

XMFLOAT3 Camera::GetRotation() const
{
	return m_rot;
}

XMVECTOR Camera::GetPositionVector() const
{
	return XMLoadFloat3( &m_pos );
}

XMVECTOR Camera::GetRotationVector() const
{
	return XMLoadFloat3( &m_rot );
}

void Camera::Update()
{
	XMFLOAT3 up, pos, look;
	XMVECTOR upV, posV, lookV;

	up.x = 0.0f;	up.y = 1.0f;	up.z = 0.0f;
	upV = XMLoadFloat3( &up );

	pos.x = m_px;	pos.y = m_py;	pos.z = m_pz;
	posV = XMLoadFloat3( &pos );

	look.x = 0.0f;	look.y = 0.0f;	look.z = 1.0f;
	lookV = XMLoadFloat3( &look );

	float pitch, yaw, roll;
	pitch = m_rx * 0.0174532925f;
	yaw = m_ry * 0.0174532925f;
	roll = m_rz * 0.0174532925f;

	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw( pitch, yaw, roll );

	lookV = XMVector3TransformCoord( lookV, rotationMatrix );
	upV = XMVector3TransformCoord( upV, rotationMatrix );

	lookV = XMVectorAdd( posV, lookV );

	m_view = XMMatrixLookAtLH( posV, lookV, upV );
}

void Camera::GetViewMatrix( XMMATRIX & view ) const
{
	view = m_view;
}

void Camera::MoveForward()
{
	m_pz += 0.05f;
}

void Camera::MoveBackward()
{
	m_pz -= 0.05f;
}

void Camera::MoveLeft()
{
	m_px -= 0.05f;
}

void Camera::MoveRight()
{
	m_px += 0.05f;
}


void Camera::MoveUp()
{
	m_py += 0.05f;
}

void Camera::MoveDown()
{
	m_py -= 0.05f;
}
