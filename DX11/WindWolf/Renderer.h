#pragma once

#include "D3DContext.h"
#include "WShaderMan.h"
#include "Camera.h"
#include "WScreen.h"


class Renderer
{
	DELETE_ASSIGN_COPY( Renderer )
public:
	Renderer();
	~Renderer();

	bool Init( int width, int height, HWND hWnd );
	void Terminate();
	bool Update();

private:
	bool Render();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

private:
	D3DContext* m_pD3DContext = nullptr;
	WShaderMan* m_pShaderMan = nullptr;

	Camera* m_pCamera = nullptr;
	WScreen* m_pScreen = nullptr;
};