#pragma once

class Camera
	: public Allocate16
{
public:
	Camera();
	~Camera();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void Update();

	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetRotation() const;

	XMMATRIX GetViewMatrix() const;
	void GetViewMatrix(XMMATRIX& matrix) const;
	

private:
	float m_px = 0.0f;
	float m_py = 0.0f;
	float m_pz = 0.0f;

	float m_rx = 0.0f;
	float m_ry = 0.0f;
	float m_rz = 0.0f;

	XMMATRIX m_view;
};