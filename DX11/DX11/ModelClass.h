#pragma once

class ModelClass
{
	DELETE_ASSIGN_COPY( ModelClass );

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	ModelClass();
	~ModelClass();

	bool Initialize( ID3D11Device* pDevice );
	void Shutdown();
	void Render( ID3D11DeviceContext* pDeviceContext );

	int GetIndexCount() const;

private:
	bool InitializeBuffers( ID3D11Device* pDevice );
	void ShutdownBuffers();
	void RenderBuffers( ID3D11DeviceContext* pDeviceContext );

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	int m_vertexCount;
	int m_indexCount;
};