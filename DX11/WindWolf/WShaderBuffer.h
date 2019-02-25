#pragma once

class WShaderBuffer
{
public:
	WShaderBuffer();
	~WShaderBuffer();

	bool Init(ID3D11Device * pDevice, D3D11_BUFFER_DESC* bufferDesc);
	void Terminate();

	bool Update(ID3D11DeviceContext* pDeviceContext, ColorMatrixBuffer & matrixBuffer);

	ID3D11Buffer* GetBuffer();
private:
	ID3D11Buffer* m_pBuffer = nullptr;
};