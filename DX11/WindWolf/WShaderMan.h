#pragma once

#include "WShader.h"
#include "WShaderBuffer.h"

class WShaderMan
{
	DELETE_ASSIGN_COPY(WShaderMan);
public:
	WShaderMan();
	~WShaderMan();

	bool Init(ID3D11Device* pDevice);
	void Terminate();

	void SetShader(ID3D11DeviceContext* pDeviceContext, SHADER_TYPE type);

#pragma region SHADER_TYPE_UPDATE
	bool UpdateColorShader( ID3D11DeviceContext* pDeviceContext, ColorMatrixBuffer& matrixBuffer);
#pragma endregion

private:
#pragma region SHADER_TYPE_INIT
	bool InitColorShader(ID3D11Device* pDevice);
	bool InitTextureShader(ID3D11Device* pDevice);
#pragma endregion



private:
	WVertexShader m_aVertexShader[SHADER_TYPE::COUNT];
	WPixelShader m_aPixelShader[SHADER_TYPE::COUNT];
	WShaderBuffer m_aShaderBuffer[SHADER_TYPE::COUNT];
};