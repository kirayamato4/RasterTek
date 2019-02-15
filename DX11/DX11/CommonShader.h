#pragma once

struct MatrixBuffer
{
	XMMATRIX _world;
	XMMATRIX _view;
	XMMATRIX _projection;

	MatrixBuffer();
};

struct CameraBuffer
{
	XMVECTOR _position;

	CameraBuffer();
};

struct LightBuffer
{
	XMFLOAT4	_ambient;
	XMFLOAT4	_diffuse;
	XMFLOAT4	_specular;
	float		_specularPower;
	XMFLOAT3	_direction;

	LightBuffer();
};

using LightParam = LightBuffer;