#include "pch.h"
#include "Light.h"

Light::Light()
	: light()
{
}

Light::~Light()
{
}

void Light::SetAmbientColor( float r, float g, float b, float a )
{
	light._ambient = XMFLOAT4( r, g, b, a );
}

void Light::SetDiffuseColor( float r, float g, float b, float a )
{
	light._diffuse = XMFLOAT4( r, g, b, a );
}

void Light::SetSpecularColor( float r, float g, float b, float a )
{
	light._specular = XMFLOAT4( r, g, b, a );
}

void Light::SetSpecularPower( float specularPower )
{
	light._specularPower = specularPower;
}

void Light::SetDirection( float x, float y, float z )
{
	light._direction = XMFLOAT3( x, y, z );
}

XMFLOAT4 Light::GetAmbientColor() const
{
	return light._ambient;
}

XMFLOAT4 Light::GetDiffuseColor() const
{
	return light._diffuse;
}

XMFLOAT4 Light::GetSpecularColor() const
{
	return light._specular;
}

float Light::GetSpecularPower() const
{
	return light._specularPower;
}

XMFLOAT3 Light::GetDirectoin() const
{
	return light._direction;
}

LightParam Light::GetLightBuffer() const
{
	return light;
}
