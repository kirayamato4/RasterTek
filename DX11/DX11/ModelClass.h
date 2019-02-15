#pragma once

#include "TextureClass.h"

class ModelClass
{
	DELETE_ASSIGN_COPY( ModelClass );

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	ModelClass();
	~ModelClass();

	bool Initialize( ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const wchar_t* fileName );
	void Shutdown();
	void Render( ID3D11DeviceContext* pDeviceContext );

	int GetIndexCount() const;
	ID3D11ShaderResourceView* GetTexture() const;

private:
	bool InitializeBuffers( ID3D11Device* pDevice );
	void ShutdownBuffers();
	void RenderBuffers( ID3D11DeviceContext* pDeviceContext );

	bool LoadTexture( ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wchar_t* fileName );
	void ReleaseTexture();

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	int m_vertexCount;
	int m_indexCount;

	TextureClass* m_pTexture;
};