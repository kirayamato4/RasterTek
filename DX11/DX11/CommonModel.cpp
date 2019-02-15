#include "pch.h"
#include "CommonModel.h"

VertexType::VertexType()
	: _pos(), _tex(), _normal()
{
}

ModelType::ModelType()
{
	x = y = z = 0.0f;
	tu = tv = 0.0f;
	nx = ny = nz = 0.0f;
}
