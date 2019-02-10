#include "pch.h"
#include "ColorShaderClass.h"

ColorShaderClass::ColorShaderClass()
	: m_pVertexShader{ nullptr }
	, m_pPixelShader{ nullptr }
	, m_pInputLayout{ nullptr }
	, m_pMatrixBuffer{ nullptr }
{
}

ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Initialize( ID3D11Device * pDevice, HWND hWnd )
{
	return InitializeShader( pDevice, hWnd, L"ColorVertexShader.hlsl", L"ColorPixelShader.hlsl" );
}

void ColorShaderClass::Shutdown()
{
	ShutdownShader();
}

bool ColorShaderClass::Render( ID3D11DeviceContext * pDeviceContext, int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection )
{
	if( !SetShaderParameters( pDeviceContext, world, view, projection ) )
		return false;

	RenderShader( pDeviceContext, indexCount );

	return true;
}

bool ColorShaderClass::InitializeShader( ID3D11Device * pDevice, HWND hWnd, WCHAR * vsFileName, WCHAR * psFileName )
{
	HRESULT hr = E_FAIL;
	ID3D10Blob* pErrorMsg = nullptr;

#pragma region VERTEX_SHADER
	ID3D10Blob* pVertexShaderBuffer = nullptr;

	hr = D3DCompileFromFile( 
		vsFileName, 
		nullptr, nullptr, 
		"VSMain", 
		"vs_5_0", 
		D3D10_SHADER_ENABLE_STRICTNESS, 
		0, 
		&pVertexShaderBuffer, 
		&pErrorMsg 
	);

	if( FAILED( hr ) )
	{
		if( pErrorMsg )
			OutputShaderErrorMessage( pErrorMsg, hWnd, vsFileName );
		else
			MessageBox( hWnd, vsFileName, L"Missing Shader File", MB_OK );

		return false;
	}
	SAFE_RELEASE( pErrorMsg );

	hr = pDevice->CreateVertexShader( 
		pVertexShaderBuffer->GetBufferPointer(), 
		pVertexShaderBuffer->GetBufferSize(), 
		nullptr, 
		&m_pVertexShader 
	);

	HR_ERROR_RETURN( hr, L"CreateVertexShader" );
#pragma endregion

#pragma region PIXEL_SHADER
	ID3D10Blob* pPixelShaderBuffer = nullptr;

	hr = D3DCompileFromFile(
		psFileName,
		nullptr, nullptr,
		"PSMain",
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&pPixelShaderBuffer,
		&pErrorMsg
	);

	if( FAILED( hr ) )
	{
		if( pErrorMsg )
			OutputShaderErrorMessage( pErrorMsg, hWnd, psFileName );
		else
			MessageBox( hWnd, psFileName, L"Missing Shader File", MB_OK );

		return false;
	}
	SAFE_RELEASE( pErrorMsg );

	hr = pDevice->CreatePixelShader(
		pPixelShaderBuffer->GetBufferPointer(),
		pPixelShaderBuffer->GetBufferSize(),
		nullptr,
		&m_pPixelShader
	);

	HR_ERROR_RETURN( hr, L"CreatePixelShader" );
#pragma endregion

#pragma region INPUT_LAYOUT
	constexpr int POLYGON_COUNT = 2;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[ POLYGON_COUNT ];
	ZeroMemory( &polygonLayout, sizeof( D3D11_INPUT_ELEMENT_DESC ) * POLYGON_COUNT );
	polygonLayout[ 0 ].SemanticName = "POSITION";
	polygonLayout[ 0 ].SemanticIndex = 0;
	polygonLayout[ 0 ].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[ 0 ].InputSlot = 0;
	polygonLayout[ 0 ].AlignedByteOffset = 0;
	polygonLayout[ 0 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[ 0 ].InstanceDataStepRate = 0;

	polygonLayout[ 1 ].SemanticName = "COLOR";
	polygonLayout[ 1 ].SemanticIndex = 0;
	polygonLayout[ 1 ].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[ 1 ].InputSlot = 0;
	polygonLayout[ 1 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[ 1 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[ 1 ].InstanceDataStepRate = 0;
	
	hr = pDevice->CreateInputLayout(
		polygonLayout,
		POLYGON_COUNT,
		pVertexShaderBuffer->GetBufferPointer(),
		pVertexShaderBuffer->GetBufferSize(),
		&m_pInputLayout
	);

	HR_ERROR_RETURN( hr, L"CreateInputLayout" );

	SAFE_RELEASE( pVertexShaderBuffer );
	SAFE_RELEASE( pPixelShaderBuffer );
#pragma endregion

#pragma region SETUP_MATRIX_BUFFER
	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory( &matrixBufferDesc, sizeof( D3D11_BUFFER_DESC ) );
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof( MatrixBufferType );
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	hr = pDevice->CreateBuffer( &matrixBufferDesc, nullptr, &m_pMatrixBuffer );
	HR_ERROR_RETURN( hr, L"ColorShaderClass CreateBuffer" );
#pragma endregion

	return true;
}

void ColorShaderClass::ShutdownShader()
{
	SAFE_RELEASE( m_pMatrixBuffer );
	SAFE_RELEASE( m_pInputLayout );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexShader );
}

void ColorShaderClass::OutputShaderErrorMessage( ID3D10Blob * pBlob, HWND hWnd, WCHAR * fileName )
{
	char* compileErrors = (char*)pBlob->GetBufferPointer();
	size_t bufferSize = pBlob->GetBufferSize();

	std::ofstream fout( "shader-error.txt" );
	for( size_t i = 0; i < bufferSize; ++i )
		fout << compileErrors[ i ];
	fout.close();

	SAFE_RELEASE( pBlob );
	MessageBox( hWnd, L"Error compile shader", fileName, MB_OK );
}

bool ColorShaderClass::SetShaderParameters( ID3D11DeviceContext * pDeviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection )
{
	HRESULT hr;

	XMMATRIX _world = XMMatrixTranspose( world );
	XMMATRIX _view = XMMatrixTranspose( view );;
	XMMATRIX _projection = XMMatrixTranspose( projection );;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr = nullptr;
	
	hr = pDeviceContext->Map( m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if( FAILED( hr ) ) 
		return false;

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = _world;
	dataPtr->view = _view;
	dataPtr->projection = _projection;

	pDeviceContext->Unmap( m_pMatrixBuffer, 0 );

	UINT bufferSlot = 0;
	pDeviceContext->VSSetConstantBuffers( bufferSlot, 1, &m_pMatrixBuffer );

	return true;
}

void ColorShaderClass::RenderShader( ID3D11DeviceContext * pDeviceContext, int indexCount )
{
	pDeviceContext->IASetInputLayout( m_pInputLayout );

	pDeviceContext->VSSetShader( m_pVertexShader, nullptr, 0 );
	pDeviceContext->PSSetShader( m_pPixelShader, nullptr, 0 );
	
	pDeviceContext->DrawIndexed( indexCount, 0, 0 );
}
