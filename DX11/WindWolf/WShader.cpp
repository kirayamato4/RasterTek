#include "pch.h"
#include "WShader.h"


WSHADER_PARAM::WSHADER_PARAM()
	: _path( L"" )
	, _entry( "" )
	, _target( "" )
{
}

WSHADER_PARAM::WSHADER_PARAM(const WSHADER_PARAM & other)
	: _path( other._path )
	, _entry( other._entry )
	, _target( other._target )
{
}

//////////////////////////////////////////////////////////////////////
// WShader
WShader::WShader()
{
}

WShader::WShader(const WSHADER_PARAM & param)
	: _param( param )
{
}

WShader::~WShader()
{
}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// WVertexShader
WVertexShader::WVertexShader()
	: WShader()
{
}

WVertexShader::WVertexShader(const WSHADER_PARAM & param)
	: WShader( param )
{
}

WVertexShader::~WVertexShader()
{
}

bool WVertexShader::Init(ID3D11Device * pDevice, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT layoutCount )
{
	if (!CompileShader(pDevice, layoutDesc, layoutCount))
		return false;

	return true;
}

void WVertexShader::Terminate()
{
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVertexShader);
}

ID3D11VertexShader * WVertexShader::GetVertexShader() const
{
	return m_pVertexShader;
}

bool WVertexShader::CompileShader(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT layoutCount)
{
	ID3D10Blob* pShaderBuffer = nullptr;
	ID3D10Blob* pErrorMsg = nullptr;

	// CreateVertexShader
	HRESULT hr = D3DCompileFromFile(
		_param._path.c_str(),
		nullptr,
		nullptr,
		_param._entry.c_str(),
		_param._target.c_str(),
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&pShaderBuffer,
		&pErrorMsg
	);

	if (FAILED(hr))
	{
		SAFE_RELEASE(pErrorMsg);
		return false;
	}

	hr = pDevice->CreateVertexShader(
		pShaderBuffer->GetBufferPointer(),
		pShaderBuffer->GetBufferSize(),
		nullptr,
		&m_pVertexShader
	);

	if (FAILED(hr))
	{
		SAFE_RELEASE(pShaderBuffer);
		return false;
	}

	// CreateInputLayout
	hr = pDevice->CreateInputLayout(
		layoutDesc,
		layoutCount,
		pShaderBuffer->GetBufferPointer(),
		pShaderBuffer->GetBufferSize(),
		&m_pInputLayout
	);

	SAFE_RELEASE(pShaderBuffer);

	return S_OK == hr;
}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// WPixelShader
WPixelShader::WPixelShader()
	: WShader()
{
}

WPixelShader::WPixelShader(const WSHADER_PARAM & param)
	: WShader(param)
{
}

WPixelShader::~WPixelShader()
{
}

bool WPixelShader::Init(ID3D11Device * pDevice)
{
	if (!CompileShader( pDevice ))
		return false;

	return true;
}

void WPixelShader::Terminate()
{
	SAFE_RELEASE(m_pPixelShader);
}

ID3D11PixelShader * WPixelShader::GetVertexShader() const
{
	return m_pPixelShader;
}

bool WPixelShader::CompileShader(ID3D11Device* pDevice)
{
	ID3D10Blob* pShaderBuffer = nullptr;
	ID3D10Blob* pErrorMsg = nullptr;

	HRESULT hr = D3DCompileFromFile(
		_param._path.c_str(),
		nullptr,
		nullptr,
		_param._entry.c_str(),
		_param._target.c_str(),
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&pShaderBuffer,
		&pErrorMsg
	);

	if (FAILED(hr))
	{
		SAFE_RELEASE(pErrorMsg);
		return false;
	}

	hr = pDevice->CreatePixelShader(
		pShaderBuffer->GetBufferPointer(),
		pShaderBuffer->GetBufferSize(),
		nullptr,
		&m_pPixelShader
	);

	SAFE_RELEASE(pShaderBuffer);

	return S_OK == hr;
}
//////////////////////////////////////////////////////////////////////
