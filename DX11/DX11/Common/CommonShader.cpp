#include "pch.h"
#include "CommonShader.h"

MatrixBuffer::MatrixBuffer()
	: _world()
	, _view()
	, _projection()
{
}

CameraBuffer::CameraBuffer()
	: _position()
{
}

LightBuffer::LightBuffer()
	: _ambient()
	, _diffuse()
	, _specular()
	, _specularPower()
	, _direction()
{
}
