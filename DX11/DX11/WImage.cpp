#include "pch.h"
#include "WImage.h"

WImage::WImage()
	: m_pVertexBuffer{ nullptr }
	, m_pIndexBuffer{ nullptr }
	, m_vertexCount{ 0 }
	, m_indexCount{ 0 }
	, m_pTexture{ nullptr }
	, m_screen{}
	, m_bitmap{}
	, m_previous{ -1, -1 }
{
}

WImage::~WImage()
{
}

bool WImage::Init( ID3D11Device * pDevice, SIZE screen, wchar_t * textureFileName, SIZE bitmap )
{
	m_screen = screen;
	m_bitmap = bitmap;

	if( !InitBuffer( pDevice ) )
		return false;

	if( !LoadTexture( pDevice, textureFileName ) )
		return false;

	return true;
}

void WImage::Terminate()
{
	TerminateTexture();
	TerminateBuffer();

}

bool WImage::Render( ID3D11DeviceContext * pDeviceContext, POINT point )
{
	if( !UpdateBuffer( pDeviceContext, point ) )
		return false;

	RenderBuffer( pDeviceContext );

	return true;
}

int WImage::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView * WImage::GetTexture() const
{
	return m_pTexture->GetTexture();
}

bool WImage::InitBuffer( ID3D11Device * pDevice )
{
	HRESULT hr = E_FAIL;

	constexpr size_t VERTEX_COUNT = 6;
	m_vertexCount = VERTEX_COUNT;
	m_indexCount = VERTEX_COUNT;

	float l = -m_bitmap.cx / 2.0f;
	float r = -l;
	float t = m_bitmap.cy / 2.0f;
	float b = -t;

#pragma region VERTEX_BUFFER
	TextureVertexType* vertices = new TextureVertexType[ m_vertexCount ];
	ZeroMemory( vertices, sizeof( TextureVertexType ) * m_vertexCount );

	//vertices[ 0 ]._pos = XMFLOAT3( l, t, 0.0f );  // Left Top
	//vertices[ 0 ]._tex = XMFLOAT2( 0.0f, 0.0f );

	//vertices[ 1 ]._pos = XMFLOAT3( r, t, 0.0f );  // Right Top
	//vertices[ 1 ]._tex = XMFLOAT2( 1.0f, 0.0f );
	//
	//vertices[ 2 ]._pos = XMFLOAT3( l, b, 0.0f );  // Left Bottom
	//vertices[ 2 ]._tex = XMFLOAT2( 0.0f, 1.0f );

	//vertices[ 3 ]._pos = XMFLOAT3( l, b, 0.0f );  // Left Bottom
	//vertices[ 3 ]._tex = XMFLOAT2( 0.0f, 1.0f );
	//
	//vertices[ 4 ]._pos = XMFLOAT3( r, t, 0.0f );  // Right Top
	//vertices[ 4 ]._tex = XMFLOAT2( 1.0f, 0.0f );

	//vertices[ 5 ]._pos = XMFLOAT3( r, b, 0.0f );  // Right Bottom
	//vertices[ 5 ]._tex = XMFLOAT2( 1.0f, 1.0f );

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof( vertexBufferDesc ) );
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof( TextureVertexType ) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory( &vertexData, sizeof( vertexData ) );
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	hr = pDevice->CreateBuffer( &vertexBufferDesc, &vertexData, &m_pVertexBuffer );
	if( FAILED( hr ) )
		return false;
#pragma endregion

#pragma region INDEX_BUFFER
	unsigned long* indices = new unsigned long[ m_indexCount ];
	for( int i = 0; i < VERTEX_COUNT; ++i )
		indices[ i ] = i;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof( indexBufferDesc ) );
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof( unsigned long ) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory( &indexData, sizeof( indexData ) );
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	hr = pDevice->CreateBuffer( &indexBufferDesc, &indexData, &m_pIndexBuffer );
	if( FAILED( hr ) )
		return false;
#pragma endregion

	SAFE_DELETE_ARRAY( vertices );
	SAFE_DELETE_ARRAY( indices );

	return true;
}

void WImage::TerminateBuffer()
{
	SAFE_RELEASE( m_pIndexBuffer );
	SAFE_RELEASE( m_pVertexBuffer );
}

bool WImage::UpdateBuffer( ID3D11DeviceContext * pDeviceContext, POINT screenLT )
{
	if( ( m_previous.x == screenLT.x ) && ( m_previous.y == screenLT.y ) )
		return true;
	
	m_previous = screenLT;

	float l = ( -m_screen.cx / 2.0f ) + screenLT.x;
	float r = l + m_bitmap.cx;
	float t = ( m_screen.cy / 2.0f ) - screenLT.y;
	float b = t - m_bitmap.cy;

	TextureVertexType* vertices = new TextureVertexType[ m_vertexCount ];
	if( nullptr == vertices )
		return false;

	vertices[ 0 ]._pos = XMFLOAT3( l, t, 0.0f );  // Left Top
	vertices[ 0 ]._tex = XMFLOAT2( 0.0f, 0.0f );

	vertices[ 1 ]._pos = XMFLOAT3( r, t, 0.0f );  // Right Top
	vertices[ 1 ]._tex = XMFLOAT2( 1.0f, 0.0f );

	vertices[ 2 ]._pos = XMFLOAT3( l, b, 0.0f );  // Left Bottom
	vertices[ 2 ]._tex = XMFLOAT2( 0.0f, 1.0f );

	vertices[ 3 ]._pos = XMFLOAT3( l, b, 0.0f );  // Left Bottom
	vertices[ 3 ]._tex = XMFLOAT2( 0.0f, 1.0f );

	vertices[ 4 ]._pos = XMFLOAT3( r, t, 0.0f );  // Right Top
	vertices[ 4 ]._tex = XMFLOAT2( 1.0f, 0.0f );

	vertices[ 5 ]._pos = XMFLOAT3( r, b, 0.0f );  // Right Bottom
	vertices[ 5 ]._tex = XMFLOAT2( 1.0f, 1.0f );

	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = pDeviceContext->Map( m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	HR_CHECK_RETURN( hr, L"WImage UpdateBuffer Fail" );
	TextureVertexType* pDataPtr = (TextureVertexType*)mappedResource.pData;
	memcpy_s( pDataPtr, sizeof( TextureVertexType ) * m_vertexCount, vertices, sizeof( TextureVertexType ) * m_vertexCount );
	pDeviceContext->Unmap( m_pVertexBuffer, 0 );

	SAFE_DELETE( vertices );
	return true;
}

void WImage::RenderBuffer( ID3D11DeviceContext * pDeviceContext )
{
	unsigned int stride = sizeof( TextureVertexType );
	unsigned int offset = 0;

	pDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	pDeviceContext->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

bool WImage::LoadTexture( ID3D11Device * pDevice, wchar_t * textureFileName )
{
	m_pTexture = new Texture;
	if( nullptr == m_pTexture )
		return false;

	if( !m_pTexture->Init( pDevice, textureFileName ) )
		return false;

	return true;
}

void WImage::TerminateTexture()
{
	SAFE_TERMINATE( m_pTexture );
}
