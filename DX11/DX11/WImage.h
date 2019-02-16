#pragma once

#include "Texture.h"

class WImage
{
	DELETE_ASSIGN_COPY( WImage );
public:
	WImage();
	~WImage();

	bool Init( ID3D11Device* pDevice, SIZE screen, wchar_t* textureFileName, SIZE bitmap );
	void Terminate();
	bool Render( ID3D11DeviceContext* pDeviceContext, POINT point );

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture() const;

private:
	bool InitBuffer( ID3D11Device* pDevice );
	void TerminateBuffer();
	bool UpdateBuffer( ID3D11DeviceContext* pDeviceContext, POINT lt );
	void RenderBuffer( ID3D11DeviceContext* pDeviceContext );

	bool LoadTexture( ID3D11Device* pDevice, wchar_t* textureFileName );
	void TerminateTexture();

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	int m_vertexCount;
	int m_indexCount;

	Texture* m_pTexture;
	SIZE m_screen;
	SIZE m_bitmap;
	POINT m_previous;
};