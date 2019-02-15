#pragma once

class TextureClass
{
	DELETE_ASSIGN_COPY( TextureClass );

	struct TgaHeader
	{
		unsigned char	data1[ 12 ];
		unsigned short	width;
		unsigned short	height;
		unsigned char	bpp;
		unsigned char	data2;
	};

public:
	TextureClass();
	~TextureClass();

	bool Initialize( ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wchar_t* fileName );
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture() const;

private:
	bool LoadTarga( char* fileName, int& width, int& height );

private:
	unsigned char*				m_pTargaData;
	ID3D11Texture2D*			m_pTexture;
	ID3D11Resource*				m_pResource;
	ID3D11ShaderResourceView*	m_pTextureView;
	
};