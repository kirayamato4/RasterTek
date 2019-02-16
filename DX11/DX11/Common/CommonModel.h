#pragma once

struct TextureVertexType
{
	XMFLOAT3 _pos;
	XMFLOAT2 _tex;

	TextureVertexType();
};

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

struct WFontData
{
	char idx;
	// font texture character left, right u
	float ltu;
	float rtu;
	// font texture character top, bottom v
	float ttv;
	float btv;
};

struct SentenceType
{
	ID3D11Buffer* _pVertexBuffer;
	ID3D11Buffer* _pIndexBuffer;
	size_t _vertexCount;
	size_t _indexCount;
	size_t _maxLength;
	float _red;
	float _green;
	float _blue;

	SentenceType();
};