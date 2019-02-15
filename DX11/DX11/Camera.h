#pragma once

class Camera
	: public Align16
{
	DELETE_ASSIGN_COPY( Camera );

public:
	Camera();
	~Camera();

	void SetPosition( float x, float y, float z );
	void SetRotation( float x, float y, float z );

	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetRotation() const;
	
	XMVECTOR GetPositionVector() const;
	XMVECTOR GetRotationVector() const;

	void Update();
	void GetViewMatrix( XMMATRIX& view ) const;

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

private:
	float m_px;
	float m_py;
	float m_pz;

	float m_rx;
	float m_ry;
	float m_rz;

	XMMATRIX m_view;
	XMFLOAT3 m_pos;
	XMFLOAT3 m_rot;
};