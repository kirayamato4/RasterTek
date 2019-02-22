#include "pch.h"
#include "WShader.h"

WShader::WShader( const WSHADER_PARAM & VS, const WSHADER_PARAM & PS )
	: _VS( VS )
	, _PS( PS )
	, m_pVertexShader( nullptr )
	, m_pPixelShader( nullptr )
{
}

WShader::~WShader()
{
	Terminate();
}

bool WShader::Init( ID3D11Device * pDevice )
{
	if( !InitVertexShader( pDevice ) )
		return false;

	if( !InitPixelShader( pDevice ) )
		return false;

	return true;
}

void WShader::Terminate()
{
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexShader );
}

ID3D11VertexShader * WShader::GetVertexShader() const
{
	return m_pVertexShader;
}

ID3D11PixelShader * WShader::GetPixelShader() const
{
	return m_pPixelShader;
}

bool WShader::InitVertexShader( ID3D11Device *& pDevice )
{
	HRESULT hr = S_OK;

	ID3D10Blob* pShaderBuffer = nullptr;
	ID3D10Blob* pErrorMsg = nullptr;

	hr = D3DCompileFromFile(
		_VS._path.c_str(),
		nullptr,
		nullptr,
		_VS._entry.c_str(),
		_VS._target.c_str(),
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&pShaderBuffer,
		&pErrorMsg 
	);

	if( FAILED( hr ) )
	{
		SAFE_RELEASE( pErrorMsg );
		return false;
	}

	hr = pDevice->CreateVertexShader(
		pShaderBuffer->GetBufferPointer(),
		pShaderBuffer->GetBufferSize(),
		nullptr,
		&m_pVertexShader
	);

	SAFE_RELEASE( pShaderBuffer );

	return S_OK == hr;
}

bool WShader::InitPixelShader( ID3D11Device *& pDevice )
{
	HRESULT hr = S_OK;

	ID3D10Blob* pShaderBuffer = nullptr;
	ID3D10Blob* pErrorMsg = nullptr;

	hr = D3DCompileFromFile(
		_PS._path.c_str(),
		nullptr,
		nullptr,
		_PS._entry.c_str(),
		_PS._target.c_str(),
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&pShaderBuffer,
		&pErrorMsg
	);

	if( FAILED( hr ) )
	{
		SAFE_RELEASE( pErrorMsg );
		return false;
	}

	hr = pDevice->CreatePixelShader(
		pShaderBuffer->GetBufferPointer(),
		pShaderBuffer->GetBufferSize(),
		nullptr,
		&m_pPixelShader
	);

	SAFE_RELEASE( pShaderBuffer );

	return S_OK == hr;
}
