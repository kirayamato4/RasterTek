#pragma once

class LightClass
{
	DELETE_ASSIGN_COPY( LightClass );
public:
	LightClass();
	~LightClass();

	void SetAmbientColor( float r, float g, float b, float a );
	void SetDiffuseColor( float r, float g, float b, float a );
	void SetDirection( float x, float y, float z );
	void SetSpecularColor( float r, float g, float b, float a );
	void SetSpecularPower( float specularPower );

	XMFLOAT4 GetAmbientColor() const;
	XMFLOAT4 GetDiffuseColor() const;
	XMFLOAT3 GetDirectoin() const;
	XMFLOAT4 GetSpecularColor() const;
	float GetSpecularPower() const;

private:
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;
	XMFLOAT4 m_specularColor;
	float m_specularPower;
};