#include "pch.h"
#include "ModelClass.h"

ModelClass::ModelClass()
	: m_pVertexBuffer{ nullptr }
	, m_pIndexBuffer{ nullptr }
	, m_vertexCount{ 0 }
	, m_indexCount{ 0 }
	, m_pTexture{ nullptr }
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize( ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, char* fileName )
{
	if( !InitializeBuffers( pDevice ) )
		return false;

	if( !LoadTexture( pDevice, pDeviceContext, fileName ) )
		return false;

	return true;
}

void ModelClass::Shutdown()
{
	ReleaseTexture();

	ShutdownBuffers();
}

void ModelClass::Render( ID3D11DeviceContext * pDeviceContext )
{
	RenderBuffers( pDeviceContext );
}

int ModelClass::GetIndexCount() const
{
	return m_indexCount;
}

ID3D11ShaderResourceView * ModelClass::GetTexture() const
{
	return m_pTexture->GetTexture();
}

bool ModelClass::InitializeBuffers( ID3D11Device * pDevice )
{
	HRESULT hr = E_FAIL;

#pragma region VERTEX_BUFFER
	m_vertexCount = 3;
	VertexType* vertices = nullptr;
	vertices = new VertexType[ m_vertexCount ];
	vertices[ 0 ].position = XMFLOAT3( -1.0f, -1.0f, 0.0f );  // Bottom left.
	vertices[ 0 ].texture = XMFLOAT2( 0.0f, 1.0f );
	vertices[ 1 ].position = XMFLOAT3( 0.0f, 1.0f, 0.0f );  // Top middle.
	vertices[ 1 ].texture = XMFLOAT2( 0.5f, 0.0f );
	vertices[ 2 ].position = XMFLOAT3( 1.0f, -1.0f, 0.0f );  // Bottom right.
	vertices[ 2 ].texture = XMFLOAT2( 1.0f, 1.0f );

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof( vertexBufferDesc ) );
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof( VertexType ) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
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
	unsigned long* indices = nullptr;
	m_indexCount = 3;
	indices = new unsigned long[ m_indexCount ];
	indices[ 0 ] = 0;  // Bottom left.
	indices[ 1 ] = 1;  // Top middle.
	indices[ 2 ] = 2;  // Bottom right.

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

void ModelClass::ShutdownBuffers()
{
	SAFE_RELEASE( m_pIndexBuffer );
	SAFE_RELEASE( m_pVertexBuffer );
}

void ModelClass::RenderBuffers( ID3D11DeviceContext * pDeviceContext )
{
	unsigned int stride = sizeof( VertexType );
	unsigned int offset = 0;

	pDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	pDeviceContext->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

bool ModelClass::LoadTexture( ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, char * fileName )
{
	m_pTexture = new TextureClass();
	if( !m_pTexture->Initialize( pDevice, pDeviceContext, fileName ) )
		return false;

	return true;
}

void ModelClass::ReleaseTexture()
{
	SAFE_SHUTDOWN( m_pTexture );
}

