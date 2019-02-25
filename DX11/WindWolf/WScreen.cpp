#include "pch.h"
#include "WScreen.h"

WScreen::WScreen(int width, int height)
	: m_width( width )
	, m_height( height )
{
}

WScreen::~WScreen()
{
}

bool WScreen::Init(ID3D11Device * pDevice)
{
	if( m_width <= 0 || m_height <= 0 )
		return false;

	if (!InitBuffer(pDevice))
		return false;

	return true;
}

void WScreen::Terminate()
{
	TerminateBuffer();
}

void WScreen::Render(ID3D11DeviceContext* pDeviceContext, D3D_PRIMITIVE_TOPOLOGY topology)
{
	UINT stride = sizeof(ColorVertex);
	UINT offset = 0;

	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->IASetPrimitiveTopology(topology);

	pDeviceContext->DrawIndexed(m_indexCount, 0, 0);
}

bool WScreen::InitBuffer(ID3D11Device * pDevice)
{
	m_vertexCount = 6;
	ColorVertex* vertices = new ColorVertex[ m_vertexCount ];
	if (nullptr == vertices)
		return false;

	XMFLOAT4 color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	float w = m_width / 2.0f;
	float h = m_height / 2.0f;

	// Left Bottom
	vertices[0]._pos = XMFLOAT3(-w, -h,  0.0f);
	vertices[0]._color = color;
	// Left Top
	vertices[1]._pos = XMFLOAT3(-w,  h,  0.0f);
	vertices[1]._color = color;
	// Right Bottom
	vertices[2]._pos = XMFLOAT3( w, -h,  0.0f);
	vertices[2]._color = color;
	// Right Bottom
	vertices[3]._pos = XMFLOAT3( w, -h,  0.0f);
	vertices[3]._color = color;
	// Left Top
	vertices[4]._pos = XMFLOAT3(-w,  h,  0.0f);
	vertices[4]._color = color;
	// Right Top
	vertices[5]._pos = XMFLOAT3( w,  h,  0.0f);
	vertices[5]._color = color;

	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(vbDesc));
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth = sizeof(ColorVertex) * m_vertexCount;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(vbData));
	vbData.pSysMem = vertices;
	vbData.SysMemPitch = 0;
	vbData.SysMemSlicePitch = 0;

	HRESULT hr = pDevice->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer);
	if (FAILED(hr))
		return false;

	m_indexCount = 6;
	int* indices = new int[m_indexCount];
	for (int i = 0; i < m_indexCount; ++i)
		indices[i] = i;

	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(ibDesc));
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.ByteWidth = sizeof(int) * m_indexCount;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA ibData;
	ZeroMemory(&ibData, sizeof(ibData));
	ibData.pSysMem = indices;
	ibData.SysMemPitch = 0;
	ibData.SysMemSlicePitch = 0;

	hr = pDevice->CreateBuffer(&ibDesc, &ibData, &m_pIndexBuffer);
	if (FAILED(hr))
		return false;

	return true;
}

void WScreen::TerminateBuffer()
{
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
}
