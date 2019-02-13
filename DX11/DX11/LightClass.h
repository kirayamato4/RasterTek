#pragma once

class LightClass
{
	DELETE_ASSIGN_COPY( LightClass );
public:
	LightClass();
	~LightClass();

	void SetDiffuseColor( float r, float g, float b, float a );
	void SetDirection( float x, float y, float z );

	XMFLOAT4 GetDiffuseColor() const;
	XMFLOAT3 GetDirectoin() const;

private:
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;
};