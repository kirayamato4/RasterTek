#pragma once

struct VertexType
{
	XMFLOAT3 _pos;
	XMFLOAT2 _tex;
	XMFLOAT3 _normal;

	VertexType();
};

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;

	ModelType();
};