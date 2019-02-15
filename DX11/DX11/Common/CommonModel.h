#pragma once


struct LightVertexType
{
	XMFLOAT3 _pos;
	XMFLOAT2 _tex;
	XMFLOAT3 _normal;

	LightVertexType();
};

struct LightModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;

	LightModelType();
};