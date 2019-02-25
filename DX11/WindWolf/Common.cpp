#include "pch.h"
#include "Common.h"

ColorMatrixBuffer::ColorMatrixBuffer()
{
	_world = XMMatrixIdentity();
	_view = XMMatrixIdentity();
	_projection = XMMatrixIdentity();
}

ColorVertex::ColorVertex()
{
	_pos = XMFLOAT3();
	_color = XMFLOAT4();
}
