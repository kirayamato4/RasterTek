#include "pch.h"
#include "WShaderMan.h"

WShaderMan::WShaderMan()
{
}

WShaderMan::~WShaderMan()
{
}

bool WShaderMan::Init(ID3D11Device* pDevice)
{
	if (!InitTextureShader(pDevice))
		return false;

	if (!InitColorShader(pDevice))
		return false;

	return true;
}

void WShaderMan::Terminate()
{
	for( size_t i = 0; i < SHADER_TYPE::COUNT; ++i )
	{
		m_aPixelShader[i].Terminate();
		m_aVertexShader[i].Terminate();
		m_aShaderBuffer[i].Terminate();
	}
}

void WShaderMan::SetShader(ID3D11DeviceContext * pDeviceContext, SHADER_TYPE type)
{
	WVertexShader& vs = m_aVertexShader[type];
	WPixelShader& ps = m_aPixelShader[type];

	pDeviceContext->IASetInputLayout(vs.GetInputLayout());

	pDeviceContext->VSSetShader(vs.GetVertexShader(), nullptr, 0);
	pDeviceContext->PSSetShader(ps.GetPixelShader(), nullptr, 0);
}

bool WShaderMan::UpdateColorShader(ID3D11DeviceContext * pDeviceContext, ColorMatrixBuffer & matrixBuffer)
{
	WShaderBuffer& sb = m_aShaderBuffer[SHADER_TYPE::COLOR];

	return sb.Update( pDeviceContext, matrixBuffer );
}

bool WShaderMan::InitColorShader(ID3D11Device* pDevice)
{
	size_t type = SHADER_TYPE::COLOR;

	WSHADER_PARAM _vs;
	_vs._path = L"../_Shader/ColorVertexShader.hlsl";
	_vs._entry = "ColorVertex";
	_vs._target = "vs_5_0";

	m_aVertexShader[type] = WVertexShader(_vs);

	const UINT layoutCount = 2;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[layoutCount];
	ZeroMemory(&polygonLayout, sizeof(D3D11_INPUT_ELEMENT_DESC) * layoutCount);
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	if (!m_aVertexShader[type].Init(pDevice, polygonLayout, layoutCount))
		return false;

	WSHADER_PARAM _ps;
	_ps._path = L"../_Shader/ColorPixelShader.hlsl";
	_ps._entry = "ColorPixel";
	_ps._target = "ps_5_0";

	m_aPixelShader[type] = WPixelShader(_ps);

	if (!m_aPixelShader[type].Init(pDevice))
		return false;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(ColorMatrixBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	if (!m_aShaderBuffer[type].Init(pDevice, &bufferDesc))
		return false;

	return true;
}

bool WShaderMan::InitTextureShader(ID3D11Device* pDevice)
{
	return true;
}

