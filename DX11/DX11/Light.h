#pragma once

class Light
{
	DELETE_ASSIGN_COPY( Light );
public:
	Light();
	~Light();

	void SetAmbientColor( float r, float g, float b, float a );
	void SetDiffuseColor( float r, float g, float b, float a );
	void SetSpecularColor( float r, float g, float b, float a );
	void SetSpecularPower( float specularPower );
	void SetDirection( float x, float y, float z );

	XMFLOAT4 GetAmbientColor() const;
	XMFLOAT4 GetDiffuseColor() const;
	XMFLOAT4 GetSpecularColor() const;
	float GetSpecularPower() const;
	XMFLOAT3 GetDirectoin() const;

	LightParam GetLightBuffer() const;

private:
	LightParam light;
};