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

void LightClass::SetDiffuseColor( float r, float g, float b, float a )
{
	m_diffuseColor = XMFLOAT4( r, g, b, a );
}

void LightClass::SetDirection( float x, float y, float z )
{
	m_direction = XMFLOAT3( x, y, z );
}

XMFLOAT4 LightClass::GetDiffuseColor() const
{
	return m_diffuseColor;
}

XMFLOAT3 LightClass::GetDirectoin() const
{
	return m_direction;
}
