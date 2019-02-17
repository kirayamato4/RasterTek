#include "pch.h"
#include "WFont.h"

WFont::WFont()
	: m_pFontData{ nullptr }
	, m_pTexture{ nullptr }
{
}

WFont::~WFont()
{
}

bool WFont::Init( ID3D11Device * pDevice, const char* fontFileName, const wchar_t * textureFileName )
{
	if( !LoadFontData( fontFileName ) )
		return false;

	if( !LoadTexture( pDevice, textureFileName ) )
		return false;

	return true;
}

void WFont::Terminate()
{
	TerminateTexture();
	TerminateFontData();
}

ID3D11ShaderResourceView * WFont::GetTexture() const
{
	return m_pTexture->GetTexture();
}

void WFont::BuildVertexArray( void * vertices, const char * sentense, float drawX, float drawY )
{
	TextureVertexType* pVertexPtr = (TextureVertexType*)vertices;
	size_t sentenceLnegh = strlen( sentense );

	constexpr float font_width = 12.0f;
	constexpr float font_height = 12.0f;


	// drawY += 450;
	size_t index = 0;
	for( size_t i = 0; i < sentenceLnegh; ++i )
	{
		char letter = sentense[ i ];

		if( ' ' == letter )
		{
			drawX += font_width;
		}
		else
		{
			WFontData& data = m_pFontData[ sentense[ i ] - 33 ];
			
			pVertexPtr[ index ]._pos = XMFLOAT3( drawX, drawY, 0.0f );  // Left Top
			pVertexPtr[ index ]._tex = XMFLOAT2( data.ltu, data.ttv );
			++index;

			pVertexPtr[ index ]._pos = XMFLOAT3( drawX + font_width, drawY, 0.0f );  // Right Top
			pVertexPtr[ index ]._tex = XMFLOAT2( data.rtu, data.ttv );
			++index;

			pVertexPtr[ index ]._pos = XMFLOAT3( drawX, drawY - 2 * font_height, 0.0f );  // Left Bottom
			pVertexPtr[ index ]._tex = XMFLOAT2( data.ltu, data.btv );
			++index;

			pVertexPtr[ index ]._pos = XMFLOAT3( drawX, drawY - 2 * font_height, 0.0f );  // Left Bottom
			pVertexPtr[ index ]._tex = XMFLOAT2( data.ltu, data.btv );
			++index;

			pVertexPtr[ index ]._pos = XMFLOAT3( drawX + font_width, drawY, 0.0f );  // Right Top
			pVertexPtr[ index ]._tex = XMFLOAT2( data.rtu, data.ttv );
			++index;

			pVertexPtr[ index ]._pos = XMFLOAT3( drawX + font_width, drawY - 2 *font_height, 0.0f );  // Right Bottom
			pVertexPtr[ index ]._tex = XMFLOAT2( data.rtu, data.btv);
			++index;

			drawX = drawX + font_width;
		}
	}
}

bool WFont::LoadFontData( const char * fontData )
{
	std::ifstream file( fontData );
	if( !file.is_open() || file.fail() )
		return false;

	m_pFontData = new WFontData[ 95 ];
	if( nullptr == m_pFontData )
		return false;

	std::vector< std::string > v;
	std::string str;
	std::string temp;
	for( size_t i = 0; i < 95; ++i )
	{
		file >> str;

		v.clear();
		std::stringstream ss( str );
		while( std::getline( ss, temp, ',' ) )
			v.emplace_back( temp );

		// , 예외 처리
		if( 11 == i )
		{
			m_pFontData[ i ].ltu = std::stof( v[ 2 ] );
			m_pFontData[ i ].rtu = std::stof( v[ 3 ] );
			m_pFontData[ i ].ttv = std::stof( v[ 4 ] );
			m_pFontData[ i ].btv = std::stof( v[ 5 ] );
		}
		else
		{
			m_pFontData[ i ].ltu = std::stof( v[ 1 ] );
			m_pFontData[ i ].rtu = std::stof( v[ 2 ] );
			m_pFontData[ i ].ttv = std::stof( v[ 3 ] );
			m_pFontData[ i ].btv = std::stof( v[ 4 ] );
		}
	}

	file.close();

	return true;
}

void WFont::TerminateFontData()
{
	SAFE_DELETE( m_pFontData );
}

bool WFont::LoadTexture( ID3D11Device * pDevice, const wchar_t * textureFileName )
{
	m_pTexture = new Texture();
	if( nullptr == m_pTexture )
		return false;

	if( !m_pTexture->Init( pDevice, textureFileName ) )
		return false;

	return true;
}

void WFont::TerminateTexture()
{
	SAFE_TERMINATE( m_pTexture );
}
