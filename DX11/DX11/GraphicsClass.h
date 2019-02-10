#pragma once

#include "D3DClass.h"

namespace GCV	// Graphics Class Value
{
	const bool FULL_SCREEN = false;
	const bool VSYNC_ENABLED = true;
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.1f;
}

class GraphicsClass
{
	DELETE_ASSIGN_COPY( GraphicsClass )
public:
	GraphicsClass();
	~GraphicsClass();

	bool Initialize( int width, int height, HWND hWnd );
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_pDirect3D;
};