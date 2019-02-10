#pragma once

class CameraClass
	: public Align16
{
	DELETE_ASSIGN_COPY( CameraClass );

public:
	CameraClass();
	~CameraClass();

	void SetPosition( float x, float y, float z );
	void SetRotation( float x, float y, float z );

	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetRotation() const;

	void Render();
	void GetViewMatrix( XMMATRIX& view ) const;

private:
	float m_px;
	float m_py;
	float m_pz;

	float m_rx;
	float m_ry;
	float m_rz;

	XMMATRIX m_view;
};