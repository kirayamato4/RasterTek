#include "../pch.h"
#include "CommonModel.h"

TextureVertexType::TextureVertexType()
	: _pos(), _tex()
{
}


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

SentenceType::SentenceType()
	: _pVertexBuffer{ nullptr }
	, _pIndexBuffer{ nullptr }
	, _vertexCount{ 0 }
	, _indexCount{ 0 }
	, _maxLength{ 0 }
	, _red{ 0.0f }
	, _green{ 0.0f }
	, _blue{ 0.0f }
{
}
