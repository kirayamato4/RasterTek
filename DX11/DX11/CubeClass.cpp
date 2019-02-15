#include "pch.h"
#include "CubeClass.h"

CubeClass::VertexType::VertexType()
	: _pos(), _tex(), _normal()
{
}

CubeClass::ModelType::ModelType()
{
	x = y = z = 0.0f;
	tu = tv = 0.0f;
	nx = ny = nz = 0.0f;
}

CubeClass::CubeClass()
	: m_pVertexBuffer{ nullptr }
	, m_pIndexBuffer{ nullptr }
	, m_vertexCount{ 0 }
	, m_indexCount{ 0 }
	, m_pTexture{ nullptr }
	, m_pModel{	nullptr}
{
}

CubeClass::~CubeClass()
{
}

bool CubeClass::Initialize( ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const char * modelFileName, const wchar_t * textureFileName )
{
	if( !LoadModel( modelFileName ) )
		return false;

	if( !InitializeBuffers( pDevice ) )
		return false;

	if( !LoadTexture( pDevice, pDeviceContext, textureFileName ) )
		return false;

	return true;
}

void CubeClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
	ReleaseModel();
}

void CubeClass::Render( ID3D11DeviceContext * pDeviceContext )
{
	RenderBuffers( pDeviceContext );
}

int CubeClass::GetIndexCount() const
{
	return m_indexCount;
}

ID3D11ShaderResourceView * CubeClass::GetTexture() const
{
	return m_pTexture->GetTexture();
}

bool CubeClass::InitializeBuffers( ID3D11Device * pDevice )
{
	HRESULT hr = E_FAIL;

	VertexType* vertices = new VertexType[ m_vertexCount ];
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

void CubeClass::ShutdownBuffers()
{
	SAFE_RELEASE( m_pIndexBuffer );
	SAFE_RELEASE( m_pVertexBuffer );
}

void CubeClass::RenderBuffers( ID3D11DeviceContext * pDeviceContext )
{
	unsigned int stride = sizeof( VertexType );
	unsigned int offset = 0;

	pDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	pDeviceContext->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

bool CubeClass::LoadTexture( ID3D11Device * pDevice, ID3D11DeviceContext* pDeviceContext, const wchar_t * textureFileName )
{
	m_pTexture = new TextureClass();

	if( !m_pTexture->Initialize( pDevice, pDeviceContext, textureFileName ) )
		return false;

	return true;
}

void CubeClass::ReleaseTexture()
{
	SAFE_SHUTDOWN( m_pTexture );
}

bool CubeClass::LoadModel( const char * modelFileName )
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

	m_pModel = new ModelType[ m_vertexCount ];
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

void CubeClass::ReleaseModel()
{
	SAFE_DELETE( m_pModel );
}
