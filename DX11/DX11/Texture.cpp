#include "pch.h"
#include "Texture.h"

Texture::Texture()
	: m_pResource{ nullptr }
	, m_pTextureView{ nullptr }
{

}

Texture::~Texture()
{
}

bool Texture::Init( ID3D11Device * pDevice, const wchar_t * fileName )
{
	int width = 0, height = 0;

	HRESULT hr = E_FAIL;
	hr = CreateDDSTextureFromFile( pDevice, fileName, &m_pResource, &m_pTextureView );
	HR_CHECK_RETURN( hr, fileName );

	return true;
}

void Texture::Terminate()
{
	SAFE_RELEASE( m_pTextureView );
	SAFE_RELEASE( m_pResource );

}

ID3D11ShaderResourceView * Texture::GetTexture() const
{
	return m_pTextureView;
}

