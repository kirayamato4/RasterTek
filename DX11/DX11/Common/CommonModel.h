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

struct WAVE_HEADER
{
	char chunkID[ 4 ];
	unsigned long chunkSize;
	char format[ 4 ];
	char subChunkID[ 4 ];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char dataChunkID[ 4 ];
	unsigned long dataSize;
};