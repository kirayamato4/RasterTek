#include "pch.h"
#include "CommonModel.h"

LightVertexType::LightVertexType()
	: _pos(), _tex(), _normal()
{
}

LightModelType::LightModelType()
{
	x = y = z = 0.0f;
	tu = tv = 0.0f;
	nx = ny = nz = 0.0f;
}
