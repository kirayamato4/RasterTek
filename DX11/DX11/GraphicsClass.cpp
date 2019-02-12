#include "pch.h"
#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
	: m_pDirect3D{ nullptr }
	, m_pCamera{ nullptr }
	, m_pModel{ nullptr }
	, m_pTextureShader{ nullptr }
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

	m_pCamera = new CameraClass();
	m_pCamera->SetPosition( 0.0f, 0.0f, -5.0f );

	m_pModel = new ModelClass();
	if( !m_pModel->Initialize( m_pDirect3D->GetDevice(), m_pDirect3D->GetDeviceContext(), "stone01.tga" ) )
	{
		MessageBox( hWnd, L"ModelClass initialize fail", L"Error", MB_OK );
		return false;
	}

	m_pTextureShader = new TextureShaderClass();
	if( !m_pTextureShader->Initialize( m_pDirect3D->GetDevice(), hWnd ) )
	{
		MessageBox( hWnd, L"TextureShaderClass initialize fail", L"Error", MB_OK );
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	SAFE_SHUTDOWN( m_pTextureShader );
	SAFE_SHUTDOWN( m_pModel );
	SAFE_DELETE( m_pCamera );
	SAFE_SHUTDOWN( m_pDirect3D );
}

bool GraphicsClass::Frame()
{
	if( !Render() )
		return false;

	return true;
}

bool GraphicsClass::Render()
{
	XMMATRIX world, view, projection;

	ID3D11Device* pDevice = m_pDirect3D->GetDevice();
	ID3D11DeviceContext* pDeviceContext = m_pDirect3D->GetDeviceContext();

	m_pDirect3D->BeginScene( 0.0f, 0.0f, 1.0f, 1.0f );

	m_pCamera->Render();

	m_pDirect3D->GetWorldMatrix( world );
	m_pCamera->GetViewMatrix( view );
	m_pDirect3D->GetProjectionMatrix( projection );

	m_pModel->Render( pDeviceContext );

	if( !m_pTextureShader->Render( pDeviceContext, m_pModel->GetIndexCount(), world, view, projection, m_pModel->GetTexture() ) )
		return false;

	m_pDirect3D->EndScene();

	return true;
}
