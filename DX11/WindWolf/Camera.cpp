#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	m_view = XMMatrixIdentity();
}

Camera::~Camera()
{

}

void Camera::SetPosition(float x, float y, float z)
{
	m_px = x;
	m_py = y;
	m_pz = z;
}
void Camera::SetRotation(float x, float y, float z)
{
	m_rx = x;
	m_ry = y;
	m_rz = z;
}

void Camera::Update()
{
	XMFLOAT3 up, pos, look;
	XMVECTOR upV, posV, lookV;

	up.x = 0.0f;	up.y = 1.0f;	up.z = 0.0f;
	upV = XMLoadFloat3(&up);

	pos.x = m_px;	pos.y = m_py;	pos.z = m_pz;
	posV = XMLoadFloat3(&pos);

	look.x = 0.0f;	look.y = 0.0f;	look.z = 1.0f;
	lookV = XMLoadFloat3(&look);

	float pitch, yaw, roll;
	pitch = m_rx * 0.0174532925f;
	yaw = m_ry * 0.0174532925f;
	roll = m_rz * 0.0174532925f;

	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookV = XMVector3TransformCoord(lookV, rotationMatrix);
	upV = XMVector3TransformCoord(upV, rotationMatrix);

	lookV = XMVectorAdd(posV, lookV);

	m_view = XMMatrixLookAtLH(posV, lookV, upV);
}

XMFLOAT3 Camera::GetPosition() const
{
	return XMFLOAT3(m_px, m_py, m_pz);
}
XMFLOAT3 Camera::GetRotation() const
{
	return XMFLOAT3(m_rx, m_ry, m_rz);
}

XMMATRIX Camera::GetViewMatrix() const
{
	return m_view;
}
void Camera::GetViewMatrix(XMMATRIX& matrix) const
{
	matrix = m_view;
}