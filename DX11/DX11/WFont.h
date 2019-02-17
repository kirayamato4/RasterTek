#pragma once

#include "Texture.h"

class WFont
{
	DELETE_ASSIGN_COPY( WFont );

	enum EFONT_DATA
	{
		ASCII_INDEX = 0,
		ASCII_VALUE,
		ASCII_LEFT_U,
		ASCII_RIGHT_U
	};
public:
	WFont();
	~WFont();

	bool Init( ID3D11Device* pDevice, const char* fontFileName, const wchar_t* textureFileName );
	void Terminate();

	ID3D11ShaderResourceView* GetTexture() const;
	void BuildVertexArray( void* vertices, const char* sentense, float drawX, float drwaY );

private:
	bool LoadFontData( const char* fontData );
	void TerminateFontData();

	bool LoadTexture( ID3D11Device* pDevice, const wchar_t* textureFileName );
	void TerminateTexture();

private:
	WFontData* m_pFontData;
	Texture* m_pTexture;
};