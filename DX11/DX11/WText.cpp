#include "pch.h"
#include "WText.h"

WText::WText()
	: m_pFont{ nullptr }
	, m_pFontShader{ nullptr }
	, m_sentence1{ nullptr }
	, m_sentence2{ nullptr }
{
}

WText::~WText()
{
}

bool WText::Init( ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, HWND hWnd, int width, int height, XMMATRIX & view )
{
	m_screenWidth = width;
	m_screenHeight = height;

	m_view = view;

	m_pFont = new WFont;
	if( nullptr == m_pFont )
		return false;

	if( !m_pFont->Init( pDevice, "Resource/bitstream.csv", L"Resource/bitstream_bw.dds" ) )
		return false;

	m_pFontShader = new FontShader;
	if( !m_pFontShader->Init( pDevice, hWnd ) )
		return false;

	if( !InitSentence( &m_sentence1, 16, pDevice ) )
		return false;

	if( !InitSentence( &m_sentence2, 16, pDevice ) )
		return false;

	if( !InitSentence( &m_fps, 32, pDevice ) )
		return false;

	if( !InitSentence( &m_cpu, 32, pDevice ) )
		return false;

	return true;
}

void WText::Terminate()
{
	TerminateSentence( &m_sentence2 );
	TerminateSentence( &m_sentence1 );

	SAFE_TERMINATE( m_pFontShader );
	SAFE_TERMINATE( m_pFont );
}

bool WText::Render( ID3D11DeviceContext * pDeviceContext, const XMMATRIX & world, const XMMATRIX & ortho )
{
	if( !RenderSentence( pDeviceContext, m_sentence1, world, ortho ) )
		return false;

	if( !RenderSentence( pDeviceContext, m_sentence2, world, ortho ) )
		return false;

	if( !RenderSentence( pDeviceContext, m_fps, world, ortho ) )
		return false;

	if( !RenderSentence( pDeviceContext, m_cpu, world, ortho ) )
		return false;

	return true;
}

bool WText::SetMousePosition( const POINT & mouse, ID3D11DeviceContext * pDeviceContext )
{
	LONG x = mouse.x;
	std::string posX = "Mouse X: ";
	posX += std::to_string( x );

	if( !UpdateSentence( m_sentence1, posX.c_str(), 0, 0, 1.0f, 1.0f, 1.0f, pDeviceContext ) )
		return false;

	LONG y = mouse.y;
	std::string posY = "Mouse Y: ";
	posY += std::to_string( y );

	if( !UpdateSentence( m_sentence2, posY.c_str(), 0, 25, 1.0f, 1.0f, 1.0f, pDeviceContext ) )
		return false;

	return true;
}

bool WText::SetFPS( const int & FPS, ID3D11DeviceContext * pDeviceContext )
{
	int _FPS = FPS > 9999 ? 9999 : FPS;
	std::string fps = "Fps: ";
	fps += std::to_string( _FPS );

	XMFLOAT3 color;
	if( _FPS > 60 )
	{
		color.x = 0.0f;
		color.y = 1.0f;
		color.z = 0.0f;
	}
	else if( _FPS > 30 )
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 0.0f;
	}
	else
	{
		color.x = 1.0f;
		color.y = 0.0f;
		color.z = 0.0f;
	}

	if( !UpdateSentence( m_fps, fps.c_str(), 0, 50, color.x, color.y, color.z, pDeviceContext ) )
		return false;

	return true;
}

bool WText::SetCPU( const int & CPU, ID3D11DeviceContext * pDeviceContext )
{
	std::string scpu = "Cpu: ";
	scpu += std::to_string( CPU );
	scpu += " %";

	if( !UpdateSentence( m_cpu, scpu.c_str(), 0, 75, 0.0f, 1.0f, 0.0f, pDeviceContext ) )
		return false;

	return true;
}

bool WText::InitSentence( SentenceType ** ppSentence, size_t maxLength, ID3D11Device * pDevice )
{
	HRESULT hr = S_OK;

	const size_t COUNT = maxLength * 6;

	*ppSentence = new SentenceType;
	( *ppSentence )->_pVertexBuffer = nullptr;
	( *ppSentence )->_pIndexBuffer = nullptr;
	( *ppSentence )->_maxLength = maxLength;
	( *ppSentence )->_vertexCount = COUNT;
	( *ppSentence )->_indexCount = COUNT;


	// VERTEX BUFFER
	TextureVertexType* vertices = new TextureVertexType[ COUNT ];
	memset( vertices, 0, sizeof( TextureVertexType ) * COUNT );

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof( vertexBufferDesc ) );
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof( TextureVertexType ) * COUNT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory( &vertexData, sizeof( vertexData ) );
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	hr = pDevice->CreateBuffer( &vertexBufferDesc, &vertexData, &( *ppSentence )->_pVertexBuffer );
	if( FAILED( hr ) )
		return false;


	// INDEX BUFFER
	unsigned long* indices = new unsigned long[ COUNT ];
	for( size_t i = 0; i < COUNT; ++i )
		indices[ i ] = i;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof( indexBufferDesc ) );
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof( unsigned long ) * COUNT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory( &indexData, sizeof( indexData ) );
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	hr = pDevice->CreateBuffer( &indexBufferDesc, &indexData, &( *ppSentence )->_pIndexBuffer );
	if( FAILED( hr ) )
		return false;

	SAFE_DELETE_ARRAY( vertices );
	SAFE_DELETE_ARRAY( indices );

	return true;
}

bool WText::UpdateSentence( SentenceType * pSentence, const char * text, int x, int y, float red, float green, float blue, ID3D11DeviceContext * pDeviceContext )
{
	pSentence->_red = red;
	pSentence->_green = green;
	pSentence->_blue = blue;

	size_t textLength = strlen( text );
	if( textLength > pSentence->_maxLength )
		return false;

	const size_t COUNT = pSentence->_vertexCount;
	TextureVertexType* vertices = new TextureVertexType[ COUNT ];
	memset( vertices, 0, sizeof( TextureVertexType ) * COUNT );

	float drawX = ( -m_screenWidth / 2.0f ) + x;
	float drawY = ( m_screenHeight / 2.0f ) - y;

	m_pFont->BuildVertexArray( (void*)vertices, text, drawX, drawY );

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = pDeviceContext->Map( pSentence->_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if( FAILED( hr ) )
		return false;

	TextureVertexType* pDataPtr = (TextureVertexType*)mappedResource.pData;
	memcpy( pDataPtr, (void*)vertices, sizeof( TextureVertexType ) * COUNT );

	pDeviceContext->Unmap( pSentence->_pVertexBuffer, 0 );

	SAFE_DELETE_ARRAY( vertices );

	return true;
}

void WText::TerminateSentence( SentenceType ** ppSentence )
{
	if( *ppSentence )
	{
		SAFE_RELEASE( ( *ppSentence )->_pVertexBuffer );
		SAFE_RELEASE( ( *ppSentence )->_pIndexBuffer );
		SAFE_DELETE( *ppSentence );
	}
}

bool WText::RenderSentence( ID3D11DeviceContext * pDeviceContext, SentenceType * pSentence, const XMMATRIX & world, const XMMATRIX & ortho )
{
	unsigned stride = sizeof( TextureVertexType );
	unsigned offset = 0;

	pDeviceContext->IASetVertexBuffers( 0, 1, &pSentence->_pVertexBuffer, &stride, &offset );
	pDeviceContext->IASetIndexBuffer( pSentence->_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	MatrixBuffer matrixBuffer;
	matrixBuffer._world = world;
	matrixBuffer._view = m_view;
	matrixBuffer._projection = ortho;

	XMFLOAT4 fontColor = { pSentence->_red, pSentence->_green, pSentence->_blue, 1.0f };

	m_pFontShader->Render( pDeviceContext, pSentence->_indexCount, matrixBuffer, m_pFont->GetTexture(), fontColor );


	return true;
}
