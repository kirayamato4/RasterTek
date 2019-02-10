#include "pch.h"
#include "CameraClass.h"

CameraClass::CameraClass()
	: m_px{ 0.0f }
	, m_py{ 0.0f }
	, m_pz{ 0.0f }
	, m_rx{ 0.0f }
	, m_ry{ 0.0f }
	, m_rz{ 0.0f }
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition( float x, float y, float z )
{
	m_px = x;
	m_py = y;
	m_pz = z;
}

void CameraClass::SetRotation( float x, float y, float z )
{
	m_rx = x;
	m_ry = y;
	m_rz = z;
}

XMFLOAT3 CameraClass::GetPosition() const
{
	return XMFLOAT3( m_px, m_py, m_pz );
}

XMFLOAT3 CameraClass::GetRotation() const
{
	return XMFLOAT3( m_rx, m_ry, m_rz );
}

void CameraClass::Render()
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

void CameraClass::GetViewMatrix( XMMATRIX & view ) const
{
	view = m_view;
}
