#pragma once

#include "D3DContext.h"
#include "Camera.h"
#include "Cube.h"
#include "Light.h"
#include "LightShader.h"

namespace RendererValue	// Graphics Class Value
{
	const bool FULL_SCREEN = false;
	const bool VSYNC_ENABLED = true;
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.1f;
}

class Renderer
{
	DELETE_ASSIGN_COPY( Renderer )
public:
	Renderer();
	~Renderer();

	bool Init( int width, int height, HWND hWnd );
	void Terminate();
	bool Update();

	void CameraUpdate( unsigned int key );

private:
	bool Render( float delta );

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

private:
	D3DContext* m_pD3DContext;
	Camera* m_pCamera;
	Light* m_pLight;
	LightShader* m_pLightShader;

	Cube* m_pCube;
};