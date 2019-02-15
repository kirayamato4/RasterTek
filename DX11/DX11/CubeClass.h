#pragma once

#include "TextureClass.h"

class CubeClass
{
	DELETE_ASSIGN_COPY( CubeClass );

	struct VertexType
	{
		XMFLOAT3 _pos;
		XMFLOAT2 _tex;
		XMFLOAT3 _normal;

		VertexType();
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;

		ModelType();
	};

public:
	CubeClass();
	~CubeClass();

	bool Initialize( ID3D11Device* pDevice, ID3D11DeviceContext * pDeviceContext, const char* modelFileName, const wchar_t* textureFileName );
	void Shutdown();
	void Render( ID3D11DeviceContext* pDeviceContext );

	int GetIndexCount() const;
	ID3D11ShaderResourceView* GetTexture() const;

private:
	bool InitializeBuffers( ID3D11Device* pDevice );
	void ShutdownBuffers();
	void RenderBuffers( ID3D11DeviceContext* pDeviceContext );

	bool LoadTexture( ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wchar_t* textureFileName );
	void ReleaseTexture();

	bool LoadModel( const char* modelFileName );
	void ReleaseModel();

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	int m_vertexCount;
	int m_indexCount;

	TextureClass* m_pTexture;
	ModelType* m_pModel;
};