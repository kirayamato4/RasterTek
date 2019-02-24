#pragma once

#include "WShader.h"

enum SHADER_TYPE : size_t
{
	COLOR = 0,
	TEXTURE,
	COUNT,	// ÃÑ ½¦ÀÌ´õ Å¸ÀÔ °¹¼ö
};


class WShaderMan
{
	DELETE_ASSIGN_COPY(WShaderMan);
public:
	WShaderMan();
	~WShaderMan();

	bool Init(ID3D11Device* pDevice);
	void Terminate();

private:
#pragma region SHADER_TYPE_INIT
	bool InitColorShader(ID3D11Device* pDevice);
	bool InitTextureShader(ID3D11Device* pDevice);
#pragma endregion

private:
	WVertexShader m_aVertexShader[COUNT];
	WPixelShader m_aPixelShader[COUNT];
};