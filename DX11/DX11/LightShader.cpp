#include "pch.h"
#include "LightShader.h"

LightShader::LightShader()
	: m_pVertexShader{ nullptr }
	, m_pPixelShader{ nullptr }
	, m_pInputLayout{ nullptr }
	, m_pSamplerState{ nullptr }
	, m_pMatrixBuffer{ nullptr }
	, m_pCameraBuffer{ nullptr }
	, m_pLightBuffer{ nullptr }
{
}

LightShader::~LightShader()
{
}

bool LightShader::Init( ID3D11Device * pDevice, HWND hWnd )
{
	return InitShader( pDevice, hWnd, L"Shader/LightVertexShader.hlsl", L"Shader/LightPixelShader.hlsl" );
}

void LightShader::Terminate()
{
	TerminateShader();
}

bool LightShader::Render( ID3D11DeviceContext * pDeviceContext, int indexCount, MatrixBuffer& matrixBuffer, CameraBuffer& cameraBuffer, LightBuffer& lightBuffer, ID3D11ShaderResourceView * pTexture )
{
	if( !SetShaderParam( pDeviceContext, matrixBuffer, cameraBuffer, lightBuffer, pTexture ) )
	{
		MAKE_WARNING( L"LightShader Render Fail" );
		return false;
	}

	RenderShader( pDeviceContext, indexCount );

	return true;
}

bool LightShader::InitShader( ID3D11Device * pDevice, HWND hWnd, WCHAR * vsFileName, WCHAR * psFileName )
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
	constexpr int POLYGON_COUNT = 3;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[ POLYGON_COUNT ];
	ZeroMemory( &polygonLayout, sizeof( D3D11_INPUT_ELEMENT_DESC ) * POLYGON_COUNT );
	polygonLayout[ 0 ].SemanticName = "POSITION";
	polygonLayout[ 0 ].SemanticIndex = 0;
	polygonLayout[ 0 ].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[ 0 ].InputSlot = 0;
	polygonLayout[ 0 ].AlignedByteOffset = 0;
	polygonLayout[ 0 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[ 0 ].InstanceDataStepRate = 0;

	polygonLayout[ 1 ].SemanticName = "TEXCOORD";
	polygonLayout[ 1 ].SemanticIndex = 0;
	polygonLayout[ 1 ].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[ 1 ].InputSlot = 0;
	polygonLayout[ 1 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[ 1 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[ 1 ].InstanceDataStepRate = 0;

	polygonLayout[ 2 ].SemanticName = "NORMAL";
	polygonLayout[ 2 ].SemanticIndex = 0;
	polygonLayout[ 2 ].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[ 2 ].InputSlot = 0;
	polygonLayout[ 2 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[ 2 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[ 2 ].InstanceDataStepRate = 0;

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

#pragma region SAMPLER_STATE
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[ 0 ] = 0;
	samplerDesc.BorderColor[ 1 ] = 0;
	samplerDesc.BorderColor[ 2 ] = 0;
	samplerDesc.BorderColor[ 3 ] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = pDevice->CreateSamplerState( &samplerDesc, &m_pSamplerState );
	HR_ERROR_RETURN( hr, L"CreateSamplerState" );
#pragma endregion

#pragma region SETUP_MATRIX_BUFFER
	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory( &matrixBufferDesc, sizeof( D3D11_BUFFER_DESC ) );
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof( MatrixBuffer );
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	hr = pDevice->CreateBuffer( &matrixBufferDesc, nullptr, &m_pMatrixBuffer );
	HR_ERROR_RETURN( hr, L"LightShader CreaetMatrixBuffer" );
#pragma endregion

#pragma region SETUP_CAMERA_BUFFER
	D3D11_BUFFER_DESC cameraBufferDesc;
	ZeroMemory( &cameraBufferDesc, sizeof( cameraBufferDesc ) );
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof( CameraBuffer );
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	hr = pDevice->CreateBuffer( &cameraBufferDesc, nullptr, &m_pCameraBuffer );
	HR_ERROR_RETURN( hr, L"LightShader CreateCameraBuffer" );
#pragma endregion

#pragma region SETUP_LIGHT_BUFFER
	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory( &lightBufferDesc, sizeof( lightBufferDesc ) );
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof( LightBuffer );
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	hr = pDevice->CreateBuffer( &lightBufferDesc, nullptr, &m_pLightBuffer );
	HR_ERROR_RETURN( hr, L"LightShader CreaetLightBuffer" );

#pragma endregion;

	return true;
}

void LightShader::TerminateShader()
{
	SAFE_RELEASE( m_pLightBuffer );
	SAFE_RELEASE( m_pCameraBuffer );
	SAFE_RELEASE( m_pMatrixBuffer );

	SAFE_RELEASE( m_pSamplerState );
	SAFE_RELEASE( m_pInputLayout );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexShader );
}

void LightShader::OutputShaderErrorMessage( ID3D10Blob * pBlob, HWND hWnd, WCHAR * fileName )
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

bool LightShader::SetShaderParam( ID3D11DeviceContext * pDeviceContext, MatrixBuffer& matrixBuffer, CameraBuffer& cameraBuffer, LightBuffer& lightBuffer, ID3D11ShaderResourceView * pTexture )
{
	HRESULT hr = E_FAIL;

	matrixBuffer._world = XMMatrixTranspose( matrixBuffer._world );
	matrixBuffer._view = XMMatrixTranspose( matrixBuffer._view );
	matrixBuffer._projection = XMMatrixTranspose( matrixBuffer._projection );

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// MatrixBuffer
	MatrixBuffer* pMatrixPtr = nullptr;
	hr = pDeviceContext->Map( m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	HR_CHECK_RETURN( hr, L"SetShaderParam" );

	pMatrixPtr = (MatrixBuffer*)mappedResource.pData;
	pMatrixPtr->_world = matrixBuffer._world;
	pMatrixPtr->_view = matrixBuffer._view;
	pMatrixPtr->_projection = matrixBuffer._projection;

	pDeviceContext->Unmap( m_pMatrixBuffer, 0 );

	unsigned int bufferSlot = 0;
	pDeviceContext->VSSetConstantBuffers( bufferSlot, 1, &m_pMatrixBuffer );
	pDeviceContext->PSSetShaderResources( 0, 1, &pTexture );

	// CameraBuffer
	CameraBuffer* pCameraPtr = nullptr;
	hr = pDeviceContext->Map( m_pCameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if( FAILED( hr ) )
		return false;

	pCameraPtr = (CameraBuffer*)mappedResource.pData;
	pCameraPtr->_position = cameraBuffer._position;

	pDeviceContext->Unmap( m_pCameraBuffer, 0 );

	bufferSlot = 1;
	pDeviceContext->VSSetConstantBuffers( bufferSlot, 1, &m_pCameraBuffer );

	// LightBuffer
	LightBuffer* pLightPtr = nullptr;
	hr = pDeviceContext->Map( m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if( FAILED( hr ) )
		return false;

	pLightPtr = (LightBuffer*)mappedResource.pData;
	pLightPtr->_ambient = lightBuffer._ambient;
	pLightPtr->_diffuse = lightBuffer._diffuse;
	pLightPtr->_specular = lightBuffer._specular;
	pLightPtr->_specularPower = lightBuffer._specularPower;
	pLightPtr->_direction = lightBuffer._direction;
	
	pDeviceContext->Unmap( m_pLightBuffer, 0 );

	bufferSlot = 0;
	pDeviceContext->PSSetConstantBuffers( bufferSlot, 1, &m_pLightBuffer );
	
	return true;
}

void LightShader::RenderShader( ID3D11DeviceContext * pDeviceContext, int indexCount )
{
	pDeviceContext->IASetInputLayout( m_pInputLayout );

	pDeviceContext->VSSetShader( m_pVertexShader, nullptr, 0 );
	pDeviceContext->PSSetShader( m_pPixelShader, nullptr, 0 );

	pDeviceContext->PSSetSamplers( 0, 1, &m_pSamplerState );

	pDeviceContext->DrawIndexed( indexCount, 0, 0 );
}
