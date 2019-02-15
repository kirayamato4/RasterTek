#include "pch.h"
#include "LightClass.h"

LightClass::LightClass()
	: m_diffuseColor{}
	, m_direction{}
{
}

LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor( float r, float g, float b, float a )
{
	m_ambientColor = XMFLOAT4( r, g, b, a );
}

void LightClass::SetDiffuseColor( float r, float g, float b, float a )
{
	m_diffuseColor = XMFLOAT4( r, g, b, a );
}

void LightClass::SetDirection( float x, float y, float z )
{
	m_direction = XMFLOAT3( x, y, z );
}

void LightClass::SetSpecularColor( float r, float g, float b, float a )
{
	m_specularColor = XMFLOAT4( r, g, b, a );
}

void LightClass::SetSpecularPower( float specularPower )
{
	m_specularPower = specularPower;
}

XMFLOAT4 LightClass::GetAmbientColor() const
{
	return m_ambientColor;
}

XMFLOAT4 LightClass::GetDiffuseColor() const
{
	return m_diffuseColor;
}

XMFLOAT3 LightClass::GetDirectoin() const
{
	return m_direction;
}

XMFLOAT4 LightClass::GetSpecularColor() const
{
	return m_specularColor;
}

float LightClass::GetSpecularPower() const
{
	return m_specularPower;
}
