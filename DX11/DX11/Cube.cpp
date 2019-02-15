#include "pch.h"
#include "Cube.h"


Cube::Cube()
	: m_pVertexBuffer{ nullptr }
	, m_pIndexBuffer{ nullptr }
	, m_vertexCount{ 0 }
	, m_indexCount{ 0 }
	, m_pTexture{ nullptr }
	, m_pModel{	nullptr}
{
}

Cube::~Cube()
{
}

bool Cube::Init( ID3D11Device * pDevice, const char * modelFileName, const wchar_t * textureFileName )
{
	if( !LoadModel( modelFileName ) )
		return false;

	if( !InitBuffer( pDevice ) )
		return false;

	if( !LoadTexture( pDevice, textureFileName ) )
		return false;

	return true;
}

void Cube::Terminate()
{
	TerminateTexture();
	TerminateBuffer();
	TerminateModel();
}

void Cube::Render( ID3D11DeviceContext * pDeviceContext )
{
	RenderBuffer( pDeviceContext );
}

int Cube::GetIndexCount() const
{
	return m_indexCount;
}

ID3D11ShaderResourceView * Cube::GetTexture() const
{
	return m_pTexture->GetTexture();
}

bool Cube::InitBuffer( ID3D11Device * pDevice )
{
	HRESULT hr = E_FAIL;

	LightVertexType* vertices = new LightVertexType[ m_vertexCount ];
	unsigned long* indices = new unsigned long[ m_indexCount ];

#pragma region LOAD_FROM_MODEL_DATA
	for( int i = 0; i < m_vertexCount; ++i )
	{
		vertices[ i ]._pos = XMFLOAT3( m_pModel[ i ].x, m_pModel[ i ].y, m_pModel[ i ].z );
		vertices[ i ]._tex = XMFLOAT2( m_pModel[ i ].tu, m_pModel[ i ].tv );
		vertices[ i ]._normal = XMFLOAT3( m_pModel[ i ].nx, m_pModel[ i ].ny, m_pModel[ i ].nz );

		indices[ i ] = i;
	}
#pragma endregion

#pragma region VERTEX_BUFFER
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof( vertexBufferDesc ) );
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof( LightVertexType ) * m_vertexCount;
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

void Cube::TerminateBuffer()
{
	SAFE_RELEASE( m_pIndexBuffer );
	SAFE_RELEASE( m_pVertexBuffer );
}

void Cube::RenderBuffer( ID3D11DeviceContext * pDeviceContext )
{
	unsigned int stride = sizeof( LightVertexType );
	unsigned int offset = 0;

	pDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	pDeviceContext->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

bool Cube::LoadTexture( ID3D11Device * pDevice, const wchar_t * textureFileName )
{
	m_pTexture = new Texture();

	if( !m_pTexture->Init( pDevice, textureFileName ) )
		return false;

	return true;
}

void Cube::TerminateTexture()
{
	SAFE_TERMINATE( m_pTexture );
}

bool Cube::LoadModel( const char * modelFileName )
{
	std::ifstream file( modelFileName );
	if( !file.is_open() )
		return false;

	char input;
	do
	{
		file.get( input );
	} while( input != ':' );

	file >> m_vertexCount;
	m_indexCount = m_vertexCount;

	m_pModel = new LightModelType[ m_vertexCount ];
	do
	{
		file.get( input );
	} while( input != ':' );
	file.get( input );

	for( int i = 0; i < m_vertexCount; ++i )
	{
		file >> m_pModel[ i ].x >> m_pModel[ i ].y >> m_pModel[ i ].z;
		file >> m_pModel[ i ].tu >> m_pModel[ i ].tv;
		file >> m_pModel[ i ].nx >> m_pModel[ i ].ny >> m_pModel[ i ].nz;
	}
	file.close();

	return true;
}

void Cube::TerminateModel()
{
	SAFE_DELETE( m_pModel );
}
