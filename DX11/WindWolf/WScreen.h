#pragma once

class WScreen
{
public:
	WScreen(int width, int height);
	~WScreen();

	bool Init(ID3D11Device* pDevice);
	void Terminate();

	void Render(ID3D11DeviceContext* pDeviceContext, D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
private:
	bool InitBuffer(ID3D11Device* pDevice);
	void TerminateBuffer();

private:
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	
	int m_vertexCount = 0;
	int m_indexCount = 0;

	int m_width = 0;
	int m_height = 0;
};