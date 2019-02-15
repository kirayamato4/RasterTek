#pragma once

#include "Texture.h"

class Cube
{
	DELETE_ASSIGN_COPY( Cube );

public:
	Cube();
	~Cube();

	bool Init( ID3D11Device* pDevice, const char* modelFileName, const wchar_t* textureFileName );
	void Terminate();
	void Render( ID3D11DeviceContext* pDeviceContext );

	int GetIndexCount() const;
	ID3D11ShaderResourceView* GetTexture() const;

private:
	bool InitBuffer( ID3D11Device* pDevice );
	void TerminateBuffer();
	void RenderBuffer( ID3D11DeviceContext* pDeviceContext );

	bool LoadTexture( ID3D11Device* pDevice, const wchar_t* textureFileName );
	void TerminateTexture();

	bool LoadModel( const char* modelFileName );
	void TerminateModel();

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	int m_vertexCount;
	int m_indexCount;

	Texture* m_pTexture;
	ModelType* m_pModel;
};