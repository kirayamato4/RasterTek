#include "pch.h"
#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
	: m_pDirect3D{ nullptr }
{

}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize( int width, int height, HWND hWnd )
{
	m_pDirect3D = new D3DClass();

	if( !m_pDirect3D->Initialize( width, height, GCV::VSYNC_ENABLED, hWnd, GCV::FULL_SCREEN, GCV::SCREEN_DEPTH, GCV::SCREEN_NEAR ) )
	{
		MessageBox( hWnd, L"Could not initialize Direct3D", L"Error", MB_OK );
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if( m_pDirect3D )
	{
		m_pDirect3D->Shutdown();
		SAFE_DELETE( m_pDirect3D );
	}
}

bool GraphicsClass::Frame()
{
	if( !Render() )
		return false;

	return true;
}

bool GraphicsClass::Render()
{
	m_pDirect3D->BeginScene( 0.0f, 0.0f, 1.0f, 1.0f );

	m_pDirect3D->EndScene();

	return true;
}
