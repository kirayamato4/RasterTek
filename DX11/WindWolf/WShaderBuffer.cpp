#include "pch.h"
#include "WShaderBuffer.h"

WShaderBuffer::WShaderBuffer()
{
}

WShaderBuffer::~WShaderBuffer()
{
}

bool WShaderBuffer::Init(ID3D11Device * pDevice, D3D11_BUFFER_DESC* bufferDesc )
{
	HRESULT hr = pDevice->CreateBuffer(bufferDesc, nullptr, &m_pBuffer);

	return S_OK == hr;
}

void WShaderBuffer::Terminate()
{
	SAFE_RELEASE(m_pBuffer);
}

bool WShaderBuffer::Update(ID3D11DeviceContext * pDeviceContext, ColorMatrixBuffer & matrixBuffer)
{
	XMMATRIX _world = matrixBuffer._world;
	XMMATRIX _view = matrixBuffer._view;
	XMMATRIX _projection = matrixBuffer._projection;

	_world = XMMatrixTranspose(_world);
	_view = XMMatrixTranspose(_view);
	_projection = XMMatrixTranspose(_projection);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(mappedResource));

	ColorMatrixBuffer* dataPtr = nullptr;


	HRESULT hr = pDeviceContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
		return false;

	dataPtr = (ColorMatrixBuffer*)mappedResource.pData;

	dataPtr->_world = _world;
	dataPtr->_view = _view;
	dataPtr->_projection = _projection;

	pDeviceContext->Unmap(m_pBuffer, 0);
	
	UINT bufferSlot = 0;
	pDeviceContext->VSSetConstantBuffers(bufferSlot, 1, &m_pBuffer);

	return true;
}

ID3D11Buffer * WShaderBuffer::GetBuffer()
{
	return m_pBuffer;
}

