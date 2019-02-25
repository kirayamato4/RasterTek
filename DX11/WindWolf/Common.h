#pragma once

enum SHADER_TYPE : size_t
{
	COLOR = 0,
	TEXTURE,
	COUNT,	// �� ���̴� Ÿ�� ����
};

struct ColorMatrixBuffer
{
	XMMATRIX _world;
	XMMATRIX _view;
	XMMATRIX _projection;

	ColorMatrixBuffer();
};

struct ColorVertex
{
	XMFLOAT3 _pos;
	XMFLOAT4 _color;

	ColorVertex();
};
