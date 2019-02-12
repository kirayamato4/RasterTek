#include "pch.h"
#include "TextureClass.h"

TextureClass::TextureClass()
	: m_pTargaData{ nullptr }
	, m_pTexture{ nullptr }
	, m_pTextureView{ nullptr }
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize( ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, char * fileName )
{
	int width = 0, height = 0;

	if( !LoadTarga( fileName, width, height ) )
		return false;

	HRESULT hr = E_FAIL;

#pragma region TEXTURE2D
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory( &textureDesc, sizeof( textureDesc ) );
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	hr = pDevice->CreateTexture2D( &textureDesc, nullptr, &m_pTexture );
	if( FAILED( hr ) )
		return false;
#pragma endregion;

	unsigned int rowPitch = ( width * 4 ) * sizeof( unsigned char );
	pDeviceContext->UpdateSubresource( m_pTexture, 0, nullptr, m_pTargaData, rowPitch, 0 );


#pragma region SHADER_RESOURCE_VIEW
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof( srvDesc ) );
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = pDevice->CreateShaderResourceView( m_pTexture, &srvDesc, &m_pTextureView );
	if( FAILED( hr ) )
		return false;
#pragma endregion

	pDeviceContext->GenerateMips( m_pTextureView );

	SAFE_DELETE( m_pTargaData );

	return true;
}

void TextureClass::Shutdown()
{
	SAFE_RELEASE( m_pTextureView );
	SAFE_RELEASE( m_pTexture );

	SAFE_DELETE( m_pTargaData );
}

ID3D11ShaderResourceView * TextureClass::GetTexture() const
{
	return m_pTextureView;
}

bool TextureClass::LoadTarga( char * fileName, int & width, int & height )
{
	FILE* pFile = nullptr;
	if( 0 != fopen_s( &pFile, fileName, "rb" ) )
		return false;

	TgaHeader tgaHeader;
	ZeroMemory( &tgaHeader, sizeof( tgaHeader ) );

	// Read File Header
	if( 1 != (unsigned int)fread( &tgaHeader, sizeof( TgaHeader ), 1, pFile ) )
	{
		fclose( pFile );
		return false;
	}

	width = (int)tgaHeader.width;
	height = (int)tgaHeader.height;
	int bpp = (int)tgaHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if( 32 != bpp )
	{
		fclose( pFile );
		return false;
	}

	// Calculate the size of the 32 bit image data.
	int imageSize = width * height * 4;
	
	unsigned char* tgaImage = new unsigned char[ imageSize ];
	if( nullptr == tgaImage )
	{
		fclose( pFile );
		return false;
	}

	if( imageSize != (unsigned int)fread( tgaImage, 1, imageSize, pFile ) )
	{
		fclose( pFile );
		return false;
	}

	fclose( pFile );

	int index = 0;
	int k = ( width * height * 4 ) - ( width * 4 );

	m_pTargaData = new unsigned char[ imageSize ];
	if( nullptr == m_pTargaData ) return false;

	for( int j = 0; j < height; ++j )
	{
		for( int i = 0; i < width; ++i )
		{
			m_pTargaData[ index + 0 ] = tgaImage[ k + 2 ];	// red
			m_pTargaData[ index + 1 ] = tgaImage[ k + 1 ];	// green
			m_pTargaData[ index + 2 ] = tgaImage[ k + 0 ];	// blue
			m_pTargaData[ index + 3 ] = tgaImage[ k + 3 ];	// alpha

			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding 
		// row at the beginning of the column since its reading it in upside down.
		k -= ( width * 8 );
	}

	SAFE_DELETE( tgaImage );

	return true;
}
